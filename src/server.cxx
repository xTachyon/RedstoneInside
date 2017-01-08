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
      s = n;
    }
  }
}

void Server::addPacket(Protocol* ptr, ByteBuffer&& buffer)
{
  mPacketsToBeHandled.push(std::pair<Protocol*, ByteBuffer>(ptr, std::move(buffer)));
}

void Server::addPlayer(const std::string nick, const std::string uuid, SessionPtr session)
{
  mPlayers.emplace_back(nick, uuid, session);
}
  
} // namespace redi

