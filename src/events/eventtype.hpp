#ifndef REDI_EVENT_EVENTTYPE_HPP
#define REDI_EVENT_EVENTTYPE_HPP

namespace redi
{

enum class EventType
{
  PlayerDisconnect,
  SessionDisconnect,
  SendKeepAliveRing,
  ChatMessage
};

} // namespace redi

#endif // REDI_EVENT_EVENTTYPE_HPP