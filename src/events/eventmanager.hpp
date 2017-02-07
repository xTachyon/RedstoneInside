#ifndef REDI_EVENTS_EVENTMANAGER_HPP
#define REDI_EVENTS_EVENTMANAGER_HPP

#include <map>
#include "eventtype.hpp"
#include "event.hpp"
#include "../threadsafequeue.hpp"
#include "eventpriority.hpp"
#include "events.hpp"

namespace redi
{

class EventManager
{
public:
  
  EventManager(Server& server);
  
  void operator()();
  
  void addEvent(EventSharedPtr ptr);
  void handlePlayerJoin(EventPlayerJoin& event);
  void handlePlayerDisconnect(EventPlayerDisconnect& event);
  void handleSessionDisconnect(EventSessionDisconnect& event);
  void handleChatMessage(EventChatMessage& event);
  
private:
  
  ThreadSafeQueue<EventSharedPtr> mEvents;
  Server& mServer;
};
  
} // namespace redi

#endif // REDI_EVENTS_EVENTMANAGER_HPP