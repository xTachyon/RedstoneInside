#ifndef REDI_EVENTS_PRIORITY
#define REDI_EVENTS_PRIORITY

#include <cstdint>
#include <functional>
#include <memory>

namespace redi
{

enum class EventPriority : std::uint8_t
{
  Lowest,
  Low,
  Normal,
  High,
  Highest,
  Monitor
};

} // namespace redi

#endif // REDI_EVENTS_PRIORITY