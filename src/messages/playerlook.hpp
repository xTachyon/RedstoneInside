#ifndef REDI_PLAYERLOOK_HPP
#define REDI_PLAYERLOOK_HPP

#include "event.hpp"
#include "../player.hpp"
#include "../playerposition.hpp"

namespace redi {

struct EventPlayerLook : public Event, public PlayerLook {
  bool onGround;
  Player& player;

  EventPlayerLook(Player& player, float yaw = 0.0f, float pitch = 0.0f,
                  bool onGround = true)
      : Event(EventType::PlayerLook), PlayerLook(yaw, pitch),
        onGround(onGround), player(player) {}
};

} // namespace redi

#endif // REDI_PLAYERLOOK_HPP
