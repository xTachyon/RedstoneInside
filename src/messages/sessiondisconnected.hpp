#pragma once

#include "event.hpp"

namespace redi {

struct EventSessionDisconnect : public Event {
  Session& session;

  EventSessionDisconnect(Session& s)
      : Event(EventType::SessionDisconnect), session(s) {}
};

} // namespace redi