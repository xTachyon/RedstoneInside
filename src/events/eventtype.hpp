#ifndef REDI_EVENT_TYPE
#define REDI_EVENT_TYPE

#include <cstdint>
#include <functional>
#include <memory>

namespace redi
{

enum class EventType : std::uint16_t
{
  SomethingHappened,
  ClientConnected,
  WeatherChange
};

enum class EventPriority : std::uint8_t
{
  Lowest,
  Low,
  Normal,
  High,
  Highest,
  Monitor
};

class Observer;

struct EventData
{
  EventType type;
  EventPriority priority;
  
  EventData(EventType type = EventType::SomethingHappened, EventPriority priority = EventPriority::Normal)
        : type(type), priority(priority) {}
};

} // namespace redi

#endif // REDI_EVENT_TYPE
