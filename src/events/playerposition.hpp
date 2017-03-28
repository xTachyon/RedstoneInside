#ifndef REDI_EVENTS_PLAYERPOSITION_HPP
#define REDI_EVENTS_PLAYERPOSITION_HPP

#include "event.hpp"
#include "../player.hpp"
#include "../vectorn.hpp"

namespace redi {

struct EventPlayerPosition : public Event, public Vector3d {
  bool onGround;
  Player& player;

  EventPlayerPosition(Player& player, double x = 0.0, double y = 0.0,
                      double z = 0.0, bool onGround = true)
      : Event(EventType::PlayerPosition), Vector3d(x, y, z), onGround(onGround),
        player(player) {}
};

} // namespace redi

#endif // REDI_EVENTS_PLAYERPOSITION_HPP