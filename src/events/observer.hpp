#ifndef REDI_EVENT_OBSERVER
#define REDI_EVENT_OBSERVER

#include <functional>
#include "eventtype.hpp"
#include "event.hpp"

namespace redi
{

class EventManager;

class Observer
{
public:

  virtual ~Observer() {}

  void registerManager(EventManager& m)
  {
    manager = std::addressof(m);
  }
  
  void registerManager(EventManager* m)
  {
    manager = m;
  }

  virtual void onEnable() {}
  virtual void onDisable() {}
  
  virtual void onWeatherChange(WeatherChangeEvent&) {}

protected:

  EventManager* manager;
};

} // namespace redi

#endif // REDI_EVENT_OBSERVER