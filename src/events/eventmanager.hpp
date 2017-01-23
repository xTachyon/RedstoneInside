#ifndef REDI_EVENTS_EVENTMANAGER_HPP
#define REDI_EVENTS_EVENTMANAGER_HPP

#include <map>
#include <boost/signals2.hpp>
#include "eventtype.hpp"
#include "functor.hpp"
#include "event.hpp"
#include "../threadsafequeue.hpp"
#include "eventpriority.hpp"

namespace redi
{

struct EventData
{
  std::unique_ptr<FunctorBase> callback;
  EventPriority priority;
};

class EventManager
{
  public:
  
  EventManager() = default;
  

  
  private:
  
//  std::map<EventType, std::vector<boost::signals2::signal<void>>> mCallbacks;
  ThreadSafeQueue<EventPtr> mEvents;
};
  
} // namespace redi

#endif // REDI_EVENTS_EVENTMANAGER_HPP