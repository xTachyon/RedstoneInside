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

Server::Server() : config("server.properties"), workIoServiceWork(workIoService),
                   mListener(std::make_shared<ConnectionListener>(networkIoService, static_cast<std::uint16_t>(config.port), *this)),
                   mEntityCount(0), mOnlinePlayers(0), mCommandManager(*this),
                   mChatManager(*this, mCommandManager), mEventManager(*this),
                   mRediCommands(mCommandManager), mUniqueLock(mCondVarMutex)
{
  fs::create_directories("players");
  fs::create_directories("worlds");
  
  addWorld("world", "worlds/world");
  
  mListener->listen();
  for (std::size_t i = 0; i < AsioThreadsNumber; ++i)
  {
    mAsioThreads.emplace_back([&]()
                              {
                                networkIoService.run();
                                Logger::debug((boost::format("Asio network thread id %1% stopped") % std::this_thread::get_id()).str());
                              });
    Logger::debug((boost::format("Asio network thread id %1% started") % mAsioThreads[i].get_id()).str());
  }
  
  for (std::size_t i = 0; i < 1; ++i)
  {
    mAsioThreads.emplace_back([&]()
                              {
                                workIoService.run();
                                Logger::debug((boost::format("Asio work thread id %1% stopped") % std::this_thread::get_id()).str());
                              });
    Logger::debug((boost::format("Asio work thread id %1% started") % mAsioThreads[i].get_id()).str());
  }
  
  Logger::info("Redi has started");
}

Server::~Server()
{
  Logger::debug("Redi is stopping");
  
  networkIoService.stop();
  workIoService.stop();
  
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
    while (true)
    {
      PacketHandlerSharedPtr x;
      if (!mPacketsToBeHandle.pop(x) || !x) break;
      
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
  mPacketsToBeHandle.push(ptr);
  mCondVar.notify_one();
}

void Server::addEvent(EventUniquePtr&& ptr)
{
  mEventManager.addEvent(std::move(ptr));
  mCondVar.notify_one();
}

void Server::addWorld(const std::string& worldname, const std::string& worlddir)
{
  mWorlds.emplace_back(*this, worldname, worlddir, std::make_shared<TerrainGenerator>());
}

void Server::broadcastPacketToPlayers(ByteBufferSharedPtr ptr, std::function<bool(const Player&)> comp)
{
  std::for_each(mPlayers.begin(), mPlayers.end(), [&](PlayerSharedPtr& player)
  {
    if (comp(*player))
    {
      player->sendPacket(ByteBuffer(*ptr));
    }
  });
}

Player* Server::findPlayer(const std::string& name)
{
  Player* ptr = nullptr;
  
  for (auto& index : mPlayers)
  {
    if (util::noCaseCompareEqual(name, index->getUsername()))
    {
      ptr = index.get();
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
    i->kick(reason);
  }
}

bool Server::toAllPlayersExcept(const Player& player, const Player& except)
{
  return player != except;
}
  
} // namespace redi

