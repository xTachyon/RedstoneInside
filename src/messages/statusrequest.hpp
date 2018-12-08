#pragma once

#include "event.hpp"
#include "../session.hpp"

namespace redi {

struct EventStatusRequest : public Event {
  Session& session;

  EventStatusRequest(Session& session)
      : Event(EventType::StatusRequest), session(session) {}
};

} // namespace redi