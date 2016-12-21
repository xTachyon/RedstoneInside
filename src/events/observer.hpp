#ifndef REDI_EVENT_OBSERVER
#define REDI_EVENT_OBSERVER

#include <functional>
#include "eventtype.hpp"

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

  virtual void onEnable() {}
  virtual void onDisable() {}

protected:

  EventManager* manager;
};

} // namespace redi

#endif // REDI_EVENT_OBSERVER
