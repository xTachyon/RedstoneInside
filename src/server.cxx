#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/format.hpp>
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
    
    while (!mActions.empty())
    {
      auto x = mActions.pop();
      
      if (x)
      {
        switch (x->getType())
        {
        case EventType::PlayerDisconnected:
        {
          const EventPlayerDisconnected& event = x->get<EventPlayerDisconnected>();
          Player* player = &event.player;
          if (player)
          {
            mConnectedClients.remove_if([&](const Session& ar)
                                        {
                                          return player->getSession() == ar;
                                        });
            mPlayers.remove_if([&](const Player& p)
                               {
                                 return std::addressof(p) == player;
                               });
            player->getWorld().deletePlayer(player);
            --mOnlinePlayers;
          }
        }
          break;
        
        case EventType::SessionDisconnected:
        {
          const EventSessionDisconnected& event = x->get<EventSessionDisconnected>();
          mPlayers.remove_if([&](const Player& p)
                             {
                               return p.getSession() == event.session;
                             });
          mConnectedClients.remove_if([&](const Session& ar)
                                      {
                                        return event.session == ar;
                                      });
        }
          break;
        
        case EventType::SendKeepAliveRing:
        {
          const EventSendKeepAliveRing& event = x->get<EventSendKeepAliveRing>();
          Protocol* p = event.session.getProtocolPtr();
          if (p) p->sendKeepAlive();
        }
          break;
        }
      }
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    n = util::getUnixTimestamp();
    if (n > s + 9)
    {
      Logger::debug((boost::format("Number of connections: %1% --- Number of players: %2%")
                    % std::distance(mConnectedClients.begin(), mConnectedClients.end())
                    % std::distance(mPlayers.begin(), mPlayers.end())).str());
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
  std::string uuid = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
  
  mPlayers.emplace_back(nick, uuid, session, getNewEntityID(), this, &mWorlds.back());
  Player& player = mPlayers.back();
  Protocol& protocol = player.getSession().getProtocol();
  ++mOnlinePlayers;
  
  player.getSession().setPlayer(player);
  player.getWorld().addPlayer(&player);
  
  protocol.sendSetCompression();
  protocol.sendLoginSucces(nick, uuid);
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

void Server::addEvent(EventPtr ptr)
{
  mActions.push(ptr);
}

void Server::addWorld(const std::string& worldname, const std::string& worlddir)
{
  mWorlds.emplace_back(worldname, worlddir, std::make_shared<TerrainGenerator>());
}

Server::Server(boost::asio::io_service& io_service) : mListener(io_service, 25565, this), mIoService(io_service), mEntityCount(0), mOnlinePlayers(0)
{
  addWorld("world", "world/region");
}
  
} // namespace redi

