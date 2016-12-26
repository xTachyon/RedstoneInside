#ifndef REDI_EVENT_EVENT
#define REDI_EVENT_EVENT

#include "eventtype.hpp"

namespace redi
{

class Event
{
public:

  Event(EventType type) : mType(type), mIsCancelled(false) {}
  
  operator bool() const { return !mIsCancelled; }
  
  void cancel() { mIsCancelled = true; }
  bool isCancelled() const { return mIsCancelled; }
  EventType getType() const { return mType; }
  
private:
  
  EventType mType;
  bool mIsCancelled;
};

class WeatherChangeEvent : public Event
{
public:
  
  static constexpr EventType type = EventType::WeatherChange;
  
  WeatherChangeEvent() : Event(type) {}
};

} // namespace redi

#endif //REDI_EVENT_EVENT
