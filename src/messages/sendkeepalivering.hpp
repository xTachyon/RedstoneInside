#pragma once

#include "event.hpp"
#include "../session.hpp"

namespace redi {

struct EventSendKeepAliveRing : public Event {
  Session& session;

  EventSendKeepAliveRing(Session& session)
      : Event(EventType::SendKeepAliveRing), session(session) {}
};

} // namespace redi