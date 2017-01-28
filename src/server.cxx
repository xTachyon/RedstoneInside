#include <boost/format.hpp>
#include "util/util.hpp"
#include "logger.hpp"
#include "exceptions.hpp"
#include "server.hpp"

namespace redi
{

void Server::run()
{
  std::int32_t s = 0, n;

  while (true)
  {
    while (!mPacketsToBeHandled.empty())
    {
      auto x = mPacketsToBeHandled.pop();
      x.first->handlePacket(x.second);
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

Server::Server(boost::asio::io_service& io_service) : config("server.properties"), mListener(io_service, static_cast<std::uint16_t>(config.port), this), mIoService(io_service), mEntityCount(0), mOnlinePlayers(0),
                                                      mChatManager(*this), mEventManager(*this)
{
  addWorld("world", "world/region");
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

