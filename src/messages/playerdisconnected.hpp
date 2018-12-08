#pragma once

#include "event.hpp"
#include "../player.hpp"

namespace redi {

struct EventPlayerDisconnect : public Event {
  Player& player;

  EventPlayerDisconnect(Player& player)
      : Event(EventType::PlayerDisconnect), player(player) {}
};

} // namespace redi