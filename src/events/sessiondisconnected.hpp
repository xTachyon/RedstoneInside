#ifndef REDI_EVENTS_SESSIONDISCONNECTED_HPP
#define REDI_EVENTS_SESSIONDISCONNECTED_HPP

#include "event.hpp"

namespace redi
{

struct EventSessionDisconnect : public Event
{
  Session& session;
  
  EventSessionDisconnect(Session& s) : Event(EventType::SessionDisconnect), session(s) {}
};
  
} // namespace redi

#endif // REDI_EVENTS_SESSIONDISCONNECTED_HPP