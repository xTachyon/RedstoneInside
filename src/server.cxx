#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "server.hpp"
#include "util/util.hpp"
#include "logger.hpp"

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
    
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    n = util::getUnixTimestamp();
    if (n > s + 2)
    {
      Logger::info("Number of connections: " + std::to_string(std::distance(mConnectedClients.begin(), mConnectedClients.end())));
      Logger::info("Number of players: " + std::to_string(std::distance(mPlayers.begin(), mPlayers.end())));
      s = n;
    }
  }
}

void Server::addPacket(Protocol* ptr, ByteBuffer&& buffer)
{
  mPacketsToBeHandled.push(std::pair<Protocol*, ByteBuffer>(ptr, std::move(buffer)));
}

void Server::addPlayer(const std::string nick, SessionPtr session)
{
  std::string uuid = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
  
  mPlayers.emplace_back(nick, uuid, session, getNewEntityID(), this);
  Player& player = mPlayers.back();
  Protocol& protocol = player.getSession().getProtocol();
  
  player.getSession().setPlayer(player);
  
  protocol.sendSetCompression();
  protocol.sendLoginSucces(nick, uuid);
  protocol.sendJoinGame(mPlayers.back());
  protocol.sendSpawnPosition();
  protocol.sendPlayerAbilities();
  protocol.sendPlayerPositionAndLook();
}
  
} // namespace redi

