#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "util/util.hpp"
#include "logger.hpp"
#include "exceptions.hpp"
#include "player.hpp"
#include "server.hpp"

namespace fs = boost::filesystem;

namespace redi
{

Server::Server() : config("server.properties"), mListener(std::make_shared<ConnectionListener>(mIoService, static_cast<std::uint16_t>(config.port), *this)),
                   mEntityCount(0), mOnlinePlayers(0), mCommandManager(*this), mChatManager(*this, mCommandManager), mEventManager(*this),
                   mRediCommands(mCommandManager), mUniqueLock(mCondVarMutex), mReady(false)
{
  addWorld("world", "world/region");
  fs::create_directories("players");
  
  mListener->listen();
  for (std::size_t i = 0; i < AsioThreadsNumber; ++i)
  {
    mAsioThreads.emplace_back([&]()
                              {
                                mIoService.run();
                              });
  }
  
}

Server::~Server()
{
  mIoService.stop();
  
  for (auto& index : mAsioThreads)
  {
    index.join();
  }
}

void Server::run()
{
  using namespace std::chrono_literals;

  while (true)
  {
    while (!mPacketHandlersToBe.empty())
    {
      PacketHandlerSharedPtr x = mPacketHandlersToBe.pop();
      try
      {
        x->handleOne();
      }
      catch (std::exception& e)
      {
        Logger::error(e.what());
        // Just ignore everything bad.
        x->getSession().disconnect();
        // and disconnect
        // TODO: add message
      }
    }
    
    try
    {
      mEventManager();
    }
    catch (StopServer&)
    {
      closeServer("Server is closing");
      mEventManager();
      return;
    }
    catch (std::exception&)
    {
      throw;
    }
    
    mCondVar.wait(mUniqueLock);
  }
}

void Server::addPacket(PacketHandlerSharedPtr ptr)
{
  mPacketHandlersToBe.push(ptr);
  mCondVar.notify_one();
}

void Server::addEvent(EventSharedPtr ptr)
{
  mEventManager.addEvent(ptr);
  mCondVar.notify_one();
}

void Server::addWorld(const std::string& worldname, const std::string& worlddir)
{
  mWorlds.emplace_back(worldname, worlddir, std::make_shared<TerrainGenerator>());
}

void Server::broadcastPacketToPlayers(ByteBufferSharedPtr ptr, std::function<bool(const Player&)> comp)
{
  std::for_each(mPlayers.begin(), mPlayers.end(), [&](Player& player)
  {
    if (comp(player))
    {
      player.sendPacket(ptr);
    }
  });
}

Player* Server::findPlayer(const std::string& name)
{
  Player* ptr = nullptr;
  
  for (auto& index : mPlayers)
  {
    if (util::noCaseCompareEqual(name, index.getUsername()))
    {
      ptr = &index;
      break;
    }
  }
  
  return ptr;
}

void Server::closeServer(const std::string& reason)
{
  mListener->mIsStopping = true;
  
  for (auto& i : mPlayers)
  {
    i.kick(reason);
  }
}

bool Server::toAllPlayersExcept(const Player& player, const Player& except)
{
  return player != except;
}
  
} // namespace redi

