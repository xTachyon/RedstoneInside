#include <algorithm>
#include "eventmanager.hpp"
#include "event.hpp"

namespace redi
{

void EventManager::addObserver(Observer& ref)
{
  addObserver(std::addressof(ref));
}

void EventManager::addObserver(Observer* ptr)
{
  mEvents[ptr];
  ptr->registerManager(this);
  ptr->onEnable();
}

void EventManager::deleteObserver(Observer& ref)
{
  deleteObserver(std::addressof(ref));
}

void EventManager::deleteObserver(Observer* ptr)
{
  mEvents.erase(ptr);
  ptr->onDisable();
}

void EventManager::registerEvent(Observer& ref, EventType type, EventPriority priority)
{
  registerEvent(std::addressof(ref), type, priority);
}

void EventManager::registerEvent(Observer* ptr, EventType type, EventPriority priority)
{
  mEvents[ptr].push_back(EventData(type, priority));
}

void EventManager::triggerWeatherChange()
{
  WeatherChangeEvent event;
  trigger(&Observer::onWeatherChange, event);
  
  for (auto& index : mEvents)
  {
    auto& vec = index.second;
    
    for (EventData& i : vec)
      if (i.type == EventType::WeatherChange)
      {
        index.first->onWeatherChange(event);
        if (event.isCancelled()) return;
        break;
      }
  }
}

EventManager::~EventManager()
{
  for (auto it = mEvents.begin(); it != mEvents.end();)
  {
    auto cop = it;
    ++it;
    cop->first->onDisable();
  }
}
  
} // namespace redi