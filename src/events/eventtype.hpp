#ifndef REDI_EVENT_TYPE
#define REDI_EVENT_TYPE

#include <cstdint>

namespace redi
{

enum class EventType : std::uint16_t
{
  ClientConnected
};

} // namespace redi

#endif // REDI_EVENT_TYPE
