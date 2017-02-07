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

Server::Server() : config("server.properties"), mListener(mIoService, static_cast<std::uint16_t>(config.port), this), mEntityCount(0), mOnlinePlayers(0),
                   mCommandManager(*this), mChatManager(*this, mCommandManager), mEventManager(*this), mAcceptConnections(true), mRediCommands(mCommandManager)
{
  addWorld("world", "world/region");
  fs::create_directories("players");
}

Server::~Server()
{
  mAcceptConnections = false;
  mIoService.stop();
  mStatusConnections.clear();
  mPlayers.clear();
}

void Server::run()
{
  while (true)
  {
    while (!mPacketHandlersToBe.empty())
    {
      try
      {
        mPacketHandlersToBe.pop()->handleOne();
      }
      catch (std::exception& e)
      {
        Logger::error(e.what());
        // Just ignore everything bad.
        // Should I disconnect ?
      }
    }
    
    try
    {
      mEventManager();
    }
    catch (StopServer&)
    {
      return;
    }
    catch (std::exception&)
    {
      throw;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Server::addPacket(PacketHandlerSharedPtr ptr)
{
  mPacketHandlersToBe.push(ptr);
}

void Server::addEvent(EventSharedPtr ptr)
{
  mEventManager.addEvent(ptr);
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

bool Server::toAllPlayersExcept(const Player& player, const Player& except)
{
  return player != except;
}
  
} // namespace redi

