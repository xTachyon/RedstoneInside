#ifndef REDI_STATUSREQUEST_HPP
#define REDI_STATUSREQUEST_HPP

#include "event.hpp"
#include "../session.hpp"

namespace redi
{

struct EventStatusRequest : public Event
{
  Session& session;
  
  EventStatusRequest(Session& session) : Event(EventType::StatusRequest), session(session) {}
};
  
} // namespace redi

#endif //REDI_STATUSREQUEST_HPP
