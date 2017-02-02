#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "util/util.hpp"
#include "logger.hpp"
#include "exceptions.hpp"
#include "server.hpp"

namespace fs = boost::filesystem;

namespace redi
{

Server::Server() : config("server.properties"), mListener(mIoService, static_cast<std::uint16_t>(config.port), this), mEntityCount(0), mOnlinePlayers(0),
        mChatManager(*this), mEventManager(*this), mAcceptConnections(true)
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
  std::int32_t s = 0, n;

  while (true)
  {
    while (!mPacketsToBeHandled.empty())
    {
      auto x = mPacketsToBeHandled.pop();
      try
      {
        x.first->handlePacket(x.second);
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

    n = util::getUnixTimestamp();
    if (n > s + 9)
    {
      Logger::debug((boost::format("Number of status connections: %1% --- Number of players: %2%")
                     % std::distance(mStatusConnections.begin(), mStatusConnections.end())
                     % mOnlinePlayers).str());
      s = n;
    }
  }
}

void Server::addPacket(Protocol* ptr, ByteBuffer&& buffer)
{
  mPacketsToBeHandled.push(std::pair<Protocol*, ByteBuffer>(ptr, std::move(buffer)));
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
    if (comp(player)) player.sendPacket(ptr);
  });
}

bool Server::toAllPlayersExcept(const Player& player, const Player& except)
{
  return player != except;
}

} // namespace redi

