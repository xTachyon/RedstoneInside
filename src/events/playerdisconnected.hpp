#ifndef REDI_EVENTS_PLAYERDISCONNECTED_HPP
#define REDI_EVENTS_PLAYERDISCONNECTED_HPP

#include "event.hpp"
#include "../player.hpp"

namespace redi {

struct EventPlayerDisconnect : public Event {
  Player& player;

  EventPlayerDisconnect(Player& player)
      : Event(EventType::PlayerDisconnect), player(player) {}
};

} // namespace redi

#endif // REDI_EVENTS_PLAYERDISCONNECTED_HPP