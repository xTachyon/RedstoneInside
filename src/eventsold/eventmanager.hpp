#ifndef REDI_EVENT_EVENTMANAGER
#define REDI_EVENT_EVENTMANAGER

#include <deque>
#include <map>
#include <vector>
#include "observer.hpp"

namespace redi
{

class EventManager
{
public:
  
  ~EventManager();
  
  void addObserver(Observer& ref);
  void addObserver(Observer* ptr);
  void deleteObserver(Observer& ref);
  void deleteObserver(Observer* ptr);
  void registerEvent(Observer& ref, EventType type, EventPriority priority = EventPriority::Normal);
  void registerEvent(Observer* ptr, EventType type, EventPriority priority = EventPriority::Normal);
  
  void triggerWeatherChange();
  
private:
  
  using EventDataVector = std::vector<EventData>;
  
  std::map<Observer*, EventDataVector> mEvents;
  
  template <typename Functor, typename Ev>
  void trigger(Functor f, Ev& event)
  {
    // TODO: Implement priorities
    
    for (auto& index : mEvents)
    {
      for (EventData& i : index.second)
        if (i.type == event.getType())
      {
        (index.first->*f)(event);
        if (!event) return;
        break;
      }
    }
  }
};
  
} // namespace redi

#endif // REDI_EVENT_EVENTMANAGER