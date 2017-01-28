#ifndef REDI_PLAYERPOSITION_HPP
#define REDI_PLAYERPOSITION_HPP

#include "enums.hpp"
#include "vectorn.hpp"

namespace redi
{

struct PlayerLook
{
  float yaw;
  float pitch;
  
  PlayerLook(float yaw = 0.0f, float pitch = 0.0f) : yaw(yaw), pitch(pitch) {}
};

struct PlayerPosition : Vector3d, PlayerLook
{
  bool onGround;
  Dimension dimension;
  
  PlayerPosition(double x = 0.0, double y = 0.0, double z = 0.0, float yaw = 0.0f, float pitch = 0.0f, bool onGround = true, Dimension dimension = Dimension::Overworld)
        : Vector3d(x, y, z), PlayerLook(yaw, pitch), onGround(onGround), dimension(dimension) {}
};

} // namespace redi


#endif // REDI_PLAYERPOSITION_HPP
