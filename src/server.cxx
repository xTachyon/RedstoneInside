#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
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
      Logger::debug((boost::format("Number of connections: %1% --- Number of players: %2%")
                     % std::distance(mConnectedClients.begin(), mConnectedClients.end())
                     % mOnlinePlayers).str());
      s = n;
    }
  }
}

void Server::addPacket(Protocol* ptr, ByteBuffer&& buffer)
{
  mPacketsToBeHandled.push(std::pair<Protocol*, ByteBuffer>(ptr, std::move(buffer)));
}

void Server::addPlayer(const std::string nick, Session* session)
{
  boost::uuids::uuid uuid = boost::uuids::random_generator()();

  mPlayers.emplace_back(nick, uuid, session, getNewEntityID(), this, &mWorlds.back());
  Player& player = mPlayers.back();
  Protocol& protocol = player.getSession().getProtocol();
  ++mOnlinePlayers;

  player.getSession().setPlayer(player);
  player.getWorld().addPlayer(&player);

  protocol.sendSetCompression();
  protocol.sendLoginSucces(nick, player.getUUID());
  protocol.sendJoinGame(mPlayers.back());
  protocol.sendSpawnPosition();
  //protocol.sendPlayerAbilities();
  protocol.sendTimeUpdate();
  protocol.sendPlayerPositionAndLook();

  ChunkManager& cm = mWorlds.back().getChunkManager();
  for (std::int32_t i = -5; i < 5; ++i)
  {
    for (std::int32_t j = -5; j < 5; ++j)
    {
      Vector2i r(i, j);

      protocol.sendChunk(cm.getChunk(r), r);
    }
  }
}

void Server::addEvent(EventSharedPtr ptr)
{
  mEventManager.addEvent(ptr);
}

void Server::addWorld(const std::string& worldname, const std::string& worlddir)
{
  mWorlds.emplace_back(worldname, worlddir, std::make_shared<TerrainGenerator>());
}

Server::Server(boost::asio::io_service& io_service) : config("server.properties"), mListener(io_service, config.port, this), mIoService(io_service), mEntityCount(0), mOnlinePlayers(0),
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

std::size_t Server::getOnlinePlayersNumber() const
{
  return static_cast<std::size_t>(std::distance(mPlayers.begin(), mPlayers.end()));
}

} // namespace redi

