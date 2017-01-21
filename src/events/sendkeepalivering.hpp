#ifndef REDI_EVENTS_SENDKEEPALIVERING_HPP
#define REDI_EVENTS_SENDKEEPALIVERING_HPP

#include "event.hpp"
#include "../session.hpp"

namespace redi
{

struct EventSendKeepAliveRing : public Event
{
  Session& session;
  
  EventSendKeepAliveRing(Session& session) : Event(EventType::SendKeepAliveRing), session(session) {}
};
  
} // namespace redi

#endif // REDI_EVENTS_SENDKEEPALIVERING_HPP