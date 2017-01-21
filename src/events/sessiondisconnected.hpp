#ifndef REDI_EVENTS_SESSIONDISCONNECTED_HPP
#define REDI_EVENTS_SESSIONDISCONNECTED_HPP

#include "event.hpp"

namespace redi
{

struct EventSessionDisconnected : public Event
{
  Session& session;
  
  EventSessionDisconnected(Session& s) : Event(EventType::SessionDisconnected), session(s) {}
};
  
} // namespace redi

#endif // REDI_EVENTS_SESSIONDISCONNECTED_HPP