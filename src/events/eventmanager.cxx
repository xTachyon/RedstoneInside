#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <json.hpp>
#include "eventmanager.hpp"
#include "../server.hpp"
#include "../util/util.hpp"
#include "../protocol/packetwriter.hpp"
#include "../player.hpp"

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
      
      {
        for (Player& player : mServer.mPlayers)
        {
          if (player != event.player)
          {
            player.getProtocol().sendPlayerListItem(event.player, PlayerListItemAction::RemovePlayer);
          }
        }
      }
      
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
      
    case EventType::PlayerPosition:
      handlePlayerPosition(e->get<EventPlayerPosition>());
      break;
      
    case EventType::PlayerLook:
      handlePlayerLook(e->get<EventPlayerLook>());
      break;
      
    case EventType::PlayerPositionAndLook:
      handlePlayerPositionAndLook(e->get<EventPlayerPositionAndLook>());
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
  boost::uuids::uuid namesp = boost::lexical_cast<boost::uuids::uuid>("77e7c416-763c-4967-8291-6698b795e90a");
  boost::uuids::name_generator gen(namesp);
  boost::uuids::uuid uuid = gen(util::toLowercase(event.nick));
  
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
  player.getWorld().addPlayer(player);
  
  protocol.sendSetCompression();
  protocol.sendLoginSucces(event.nick, boost::lexical_cast<std::string>(player.getUUID()));
  protocol.sendJoinGame(mServer.mPlayers.back());
  protocol.sendSpawnPosition();
  //protocol.sendPlayerAbilities();
  protocol.sendTimeUpdate();
  protocol.sendPlayerPositionAndLook(player);
  
  ChunkManager& cm = mServer.mWorlds.back().getChunkManager();
  for (std::int32_t i = -2; i <= 2; ++i)
  {
    for (std::int32_t j = -2; j <= 2; ++j)
    {
      Vector2i r(i, j);
      
      protocol.sendChunk(cm.getChunk(r), r);
    }
  }
  
  for (Player& idx : mServer.mPlayers)
  {
    if (idx == player)
    {
      for (Player& i : mServer.mPlayers)
      {
        player.getProtocol().sendPlayerListItem(i, PlayerListItemAction::AddPlayer);
      }
    }
    else
    {
      idx.getProtocol().sendPlayerListItem(player, PlayerListItemAction::AddPlayer);
      idx.getProtocol().sendSpawnPlayerPacket(player);
    }
  }
}

void EventManager::handlePlayerDisconnect(EventPlayerDisconnect& event)
{
  Player& player = event.player;
  
  player.getWorld().deletePlayer(player);
  // First remove the player from the world
  // so we won't SIGSEGV when deferencing it after deleting
  mServer.mPlayers.remove_if([&](const Player& p)
                             {
                               return p == player;
                             });
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
  if (config.iconb64.size() != 0) j["favicon"] = config.iconb64;
  
  for (const auto& player: mServer.getOnlinePlayers())
  {
    nlohmann::json c;
    
    c["id"] = boost::lexical_cast<std::string>(player.getUUID());
    c["name"] = player.getUsername();
    
    j["players"]["sample"].push_back(c);
  }
  
  PacketWriter pkt(0x00);
  pkt.writeString(j.dump());
  pkt.commit();
  
  event.session.sendPacket(pkt, "Status Request");
}

void EventManager::handlePlayerLook(EventPlayerLook& event)
{
  PlayerPosition& p = event.player.mPosition;
  p.yaw = event.yaw;
  p.pitch = event.pitch;
  p.onGround = event.onGround;
}

void EventManager::handlePlayerPosition(EventPlayerPosition& event)
{
  PlayerPosition& p = event.player.mPosition;
  p.x = event.x;
  p.y = event.y;
  p.z = event.z;
  p.onGround = event.onGround;
}

void EventManager::handlePlayerPositionAndLook(EventPlayerPositionAndLook& event)
{
  PlayerPosition& p = event.player.mPosition;
  p.x = event.x;
  p.y = event.y;
  p.z = event.z;
  p.yaw = event.yaw;
  p.pitch = event.pitch;
  p.onGround = event.onGround;
}
  
} // namespace redi