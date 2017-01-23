#include "eventmanager.hpp"
#include "../server.hpp"

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
  mServer.mPlayers.remove_if([&](const Player& p)
                     {
                       return p.getSession() == event.session;
                     });
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
  
} // namespace redi