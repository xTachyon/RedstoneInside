#ifndef REDI_PLAYERPOSITIONANDLOOK_HPP
#define REDI_PLAYERPOSITIONANDLOOK_HPP

#include "event.hpp"
#include "../player.hpp"
#include "../playerposition.hpp"
#include "../vectorn.hpp"

namespace redi {

struct EventPlayerPositionAndLook : public Event,
                                    public Vector3d,
                                    public PlayerLook {
  bool onGround;
  Player& player;

  EventPlayerPositionAndLook(Player& player, double x = 0.0, double y = 0.0,
                             double z = 0.0, float yaw = 0.0f,
                             float pitch = 0.0f, bool onGround = true)
      : Event(EventType::PlayerPositionAndLook), Vector3d(x, y, z),
        PlayerLook(yaw, pitch), onGround(onGround), player(player) {}
};

} // namespace redi

#endif // REDI_PLAYERPOSITIONANDLOOK_HPP
