#ifndef REDI_EVENT_EVENTTYPE_HPP
#define REDI_EVENT_EVENTTYPE_HPP

namespace redi
{

enum class EventType
{
  PlayerDisconnected,
  SessionDisconnected,
  SendKeepAliveRing
};

} // namespace redi

#endif // REDI_EVENT_EVENTTYPE_HPP