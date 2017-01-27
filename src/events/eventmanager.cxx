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
    case EventType::PlayerDisconnect:
      handlePlayerDisconnect(e->get<EventPlayerDisconnect>());
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

void EventManager::handlePlayerDisconnect(EventPlayerDisconnect& event)
{
  Player& player = event.player;
  
  mServer.mConnectedClients.remove_if([&](const Session& ar)
                              {
                                return player.getSession() == ar;
                              });
  mServer.mPlayers.remove_if([&](const Player& p)
                     {
                       return p == player;
                     });
  player.getWorld().deletePlayer(&player);
  --mServer.mOnlinePlayers;
}

void EventManager::handleSessionDisconnect(EventSessionDisconnect& event)
{
  bool hasPlayer = false;
  mServer.mPlayers.remove_if([&](const Player& p)
                     {
                       hasPlayer = (event.session == p.getSession());
                       return hasPlayer;
                     });
  if (hasPlayer) --mServer.mOnlinePlayers;
  mServer.mConnectedClients.remove_if([&](const Session& ar)
                              {
                                return event.session == ar;
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