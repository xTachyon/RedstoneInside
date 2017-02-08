#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <json.hpp>
#include "eventmanager.hpp"
#include "../server.hpp"
#include "../util/util.hpp"
#include "../player.hpp"
#include "../protocol/packets/server/play/playerlistitem.hpp"

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
        for (PlayerSharedPtr& player : mServer.mPlayers)
        {
          if (*player != event.player)
          {
            packets::PlayerListItem(event.player, PlayerListItemAction::RemovePlayer).send(*player);
          }
        }
      }
      
      handlePlayerDisconnect(event);
    }
      break;
    
    case EventType::SessionDisconnect:
      handleSessionDisconnect(e->get<EventSessionDisconnect>());
      break;
//
//    case EventType::SendKeepAliveRing:
//      handleSendKeepAliveRing(e->get<EventSendKeepAliveRing>());
//      break;
//
    case EventType::ChatMessage:
      handleChatMessage(e->get<EventChatMessage>());
      break;
//
//    case EventType::StatusRequest:
//      handleStatusRequest(e->get<EventStatusRequest>());
//      break;
//
//    case EventType::PlayerPosition:
//      handlePlayerPosition(e->get<EventPlayerPosition>());
//      break;
//
//    case EventType::PlayerLook:
//      handlePlayerLook(e->get<EventPlayerLook>());
//      break;
//
//    case EventType::PlayerPositionAndLook:
//      handlePlayerPositionAndLook(e->get<EventPlayerPositionAndLook>());
//      break;
    
    default:
      break;
    }
  }
}

void EventManager::addEvent(EventSharedPtr ptr)
{
  mEvents.push(ptr);
}

void EventManager::handlePlayerJoin(EventPlayerJoin&) {}

void EventManager::handlePlayerDisconnect(EventPlayerDisconnect& event)
{
  Player& player = event.player;

  player.getWorld().deletePlayer(player);
  // First remove the player from the world
  // so we won't SIGSEGV when deferencing it after deleting
  mServer.mPlayers.remove_if([&](const PlayerSharedPtr& p)
                             {
                               return *p == player;
                             });
  --mServer.mOnlinePlayers;
}

void EventManager::handleSessionDisconnect(EventSessionDisconnect& event)
{
  mServer.mStatusConnections.remove_if([&](const SessionSharedPtr& ar)
                                       {
                                         return event.session == *ar;
                                       });
}

void EventManager::handleChatMessage(EventChatMessage& event)
{
  mServer.mChatManager(event);
}
  
} // namespace redi