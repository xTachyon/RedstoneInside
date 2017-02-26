#ifndef REDI_EVENTS_EVENTMANAGER_HPP
#define REDI_EVENTS_EVENTMANAGER_HPP

#include <map>
#include "eventtype.hpp"
#include "event.hpp"
#include "../threadsafequeue.hpp"
#include "eventpriority.hpp"
#include "events.hpp"
#include "../protocol/packets/server/play/entitylookandrelativemove.hpp"

namespace redi
{

class EventManager
{
public:
  
  EventManager(Server& server);
  
  void operator()();
  
  void addEvent(EventUniquePtr&& ptr);
  void handlePlayerJoin(EventPlayerJoin& event);
  void handlePlayerDisconnect(EventPlayerDisconnect& event);
  void handleSessionDisconnect(EventSessionDisconnect& event);
  void handleChatMessage(EventChatMessage& event);
  void handleChunkLoaded(events::EventChunkLoaded& event);
  
private:
  
  ThreadSafeQueue<EventUniquePtr> mEvents;
  Server& mServer;
};
  
} // namespace redi

#endif // REDI_EVENTS_EVENTMANAGER_HPP