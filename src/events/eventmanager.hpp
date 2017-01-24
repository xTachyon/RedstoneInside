#ifndef REDI_EVENTS_EVENTMANAGER_HPP
#define REDI_EVENTS_EVENTMANAGER_HPP

#include <map>
#include <boost/signals2.hpp>
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
  void handlePlayerDisconnect(EventPlayerDisconnect& event);
  void handleSessionDisconnect(EventSessionDisconnect& event);
  void handleSendKeepAliveRing(EventSendKeepAliveRing& event);
  void handleChatMessage(EventChatMessage& event);
  void handleStatusRequest(EventStatusRequest& event);
  
private:
  
  ThreadSafeQueue<EventSharedPtr> mEvents;
  Server& mServer;
};
  
} // namespace redi

#endif // REDI_EVENTS_EVENTMANAGER_HPP