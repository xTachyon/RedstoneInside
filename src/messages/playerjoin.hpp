#pragma once

#include <string>
#include "event.hpp"
#include "../player.hpp"

namespace redi {

struct EventPlayerJoin : public Event {
  SessionSharedPtr session;
  std::string username;

  EventPlayerJoin(SessionSharedPtr session, std::string&& nick)
      : Event(EventType::PlayerJoin), session(session),
        username(std::move(nick)) {}
};

} // namespace redi