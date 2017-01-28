#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <json.hpp>
#include "eventmanager.hpp"
#include "../server.hpp"
#include "../util/util.hpp"
#include "../protocol/packetwriter.hpp"

namespace redi
{

EventManager::EventManager(Server& server) : mServer(server) {}

void EventManager::operator()()
{
  while (!mEvents.empty())
  {
    EventSharedPtr e = mEvents.pop();
    if (!e) continue;
    
    switch (e->getType())
    {
    case EventType::PlayerJoin:
    {
      EventPlayerJoin& event = e->get<EventPlayerJoin>();
      handlePlayerJoin(event);
      mServer.mChatManager(event);
    }
      break;
    
    case EventType::PlayerDisconnect:
    {
      EventPlayerDisconnect& event = e->get<EventPlayerDisconnect>();
      mServer.mChatManager(event);
      handlePlayerDisconnect(event);
    }
      break;
    
    case EventType::SessionDisconnect:
      handleSessionDisconnect(e->get<EventSessionDisconnect>());
      break;
    
    case EventType::SendKeepAliveRing:
      handleSendKeepAliveRing(e->get<EventSendKeepAliveRing>());
      break;
    
    case EventType::ChatMessage:
      handleChatMessage(e->get<EventChatMessage>());
      break;
    
    case EventType::StatusRequest:
      handleStatusRequest(e->get<EventStatusRequest>());
      break;
    
    default:
      break;
    }
  }
}

void EventManager::addEvent(EventSharedPtr ptr)
{
  mEvents.push(ptr);
}

void EventManager::handlePlayerJoin(EventPlayerJoin& event)
{
  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  
  for (SessionUniquePtr& s : mServer.mStatusConnections) // TODO: find a better way
  {
    if (*s == event.session)
    {
      mServer.mPlayers.emplace_back(event.nick, uuid, std::move(s), mServer.getNewEntityID(), &mServer, &mServer.mWorlds.back());
      mServer.mStatusConnections.remove_if([](const SessionUniquePtr& par) -> bool
                                           {
                                             return !static_cast<bool>(par);
                                           });
      break;
    }
  }
  Player& player = mServer.mPlayers.back();
  Protocol& protocol = player.getSession().getProtocol();
  ++mServer.mOnlinePlayers;
  
  player.getSession().setPlayer(player);
  player.getWorld().addPlayer(&player);
  
  protocol.sendSetCompression();
  protocol.sendLoginSucces(event.nick, boost::lexical_cast<std::string>(player.getUUID()));
  protocol.sendJoinGame(mServer.mPlayers.back());
  protocol.sendSpawnPosition();
  //protocol.sendPlayerAbilities();
  protocol.sendTimeUpdate();
  protocol.sendPlayerPositionAndLook();
  
  ChunkManager& cm = mServer.mWorlds.back().getChunkManager();
  for (std::int32_t i = -2; i <= 2; ++i)
  {
    for (std::int32_t j = -2; j <= 2; ++j)
    {
      Vector2i r(i, j);
      
      protocol.sendChunk(cm.getChunk(r), r);
    }
  }
}

void EventManager::handlePlayerDisconnect(EventPlayerDisconnect& event)
{
  Player& player = event.player;
  
  mServer.mPlayers.remove_if([&](const Player& p)
                             {
                               return p == player;
                             });
  player.getWorld().deletePlayer(&player);
  --mServer.mOnlinePlayers;
}

void EventManager::handleSessionDisconnect(EventSessionDisconnect& event)
{
  mServer.mStatusConnections.remove_if([&](const SessionUniquePtr& ar)
                                       {
                                         return event.session == *ar;
                                       });
}

void EventManager::handleSendKeepAliveRing(EventSendKeepAliveRing& event)
{
  Protocol& p = event.session.getProtocol();
  p.sendKeepAlive();
}

void EventManager::handleChatMessage(EventChatMessage& event)
{
  mServer.mChatManager(event);
}

void EventManager::handleStatusRequest(EventStatusRequest& event)
{
  const ServerConfig& config = mServer.config;
  nlohmann::json j;
  
  j["description"]["text"] = config.motd;
  j["version"]["name"] = "RedstoneInside";
  j["version"]["protocol"] = 316;
  j["players"]["max"] = config.maxPlayers;
  j["players"]["online"] = mServer.getOnlinePlayersNumber();
  j["players"]["sample"] = nlohmann::json::array();
  j["favicon"] = config.iconb64;
  
  for (const auto& player: mServer.getOnlinePlayers())
  {
    nlohmann::json c;
    
    c["id"] = boost::lexical_cast<std::string>(player.getUUID());
    c["name"] = player.getPlayerName();
    
    j["players"]["sample"].push_back(c);
  }
  
  PacketWriter pkt(0x00);
  pkt.writeString(j.dump());
  pkt.commit();
  
  event.session.sendPacket(pkt, "Status Request");
}
  
} // namespace redi