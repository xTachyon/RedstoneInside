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
        case EventType::PlayerDC:
        {
          const EventPlayerDC& event = x->get<EventPlayerDC>();
          Player* player = event.player;
          if (player)
          {
            mConnectedClients.remove_if([&](const Session& ar)
                                        {
                                          return player->getSessionPtr() == std::addressof(ar);
                                        });
            mPlayers.remove_if([&](const Player& p)
                               {
                                 return std::addressof(p) == player;
                               });
//            EventPtr ptr(new EventSessionDC(player->getSessionPtr()));
//            addEvent(ptr);
          }
        }
          break;
        
        case EventType::SessionDC:
        {
          const EventSessionDC& event = x->get<EventSessionDC>();
          mPlayers.remove_if([&](const Player& p)
                             {
                               return p.getSessionPtr() == event.session;
                             });
          mConnectedClients.remove_if([&](const Session& ar)
                                      {
                                        return event.session == std::addressof(ar);
                                      });
        }
          break;
        
        case EventType::SendKeepAlive:
        {
          const EventSendKeepAliveRing& event = x->get<EventSendKeepAliveRing>();
          Protocol* p = event.session.getProtocolPtr();
          if (p) p->sendKeepAlive();
        }
          break;
        }
      }
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    n = util::getUnixTimestamp();
    if (n > s + 4)
    {
      Logger::info((boost::format("Number of connections: %1% --- Number of players: %2%")
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
  
  mPlayers.emplace_back(nick, uuid, session, getNewEntityID(), this);
  Player& player = mPlayers.back();
  Protocol& protocol = player.getSession().getProtocol();
  
  player.getSession().setPlayer(player);
  
  protocol.sendSetCompression();
  protocol.sendLoginSucces(nick, uuid);
  protocol.sendJoinGame(mPlayers.back());
  protocol.sendSpawnPosition();
  //protocol.sendPlayerAbilities();
  protocol.sendTimeUpdate();
  protocol.sendPlayerPositionAndLook();
  
  ChunkManager& cm = mWorlds.back().getChunkManager();
  for (std::int32_t i = -10; i < 21; ++i)
  {
    for (std::int32_t j = -10; j < 21; ++j)
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

Server::Server(boost::asio::io_service& io_service) : mListener(io_service, 25565, this), mIoService(io_service), mEntityCount(0)
{
  addWorld("world", "world/region");
}
  
} // namespace redi

