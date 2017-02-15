#ifndef REDI_PLAYERPOSITION_HPP
#define REDI_PLAYERPOSITION_HPP

#include "util/util.hpp"
#include "enums.hpp"
#include "vectorn.hpp"

namespace redi
{

struct PlayerLook
{
  float yaw;
  float pitch;
  
  PlayerLook(float yaw = 0.0f, float pitch = 0.0f) : yaw(yaw), pitch(pitch) {}
  
  void normalize()
  {
    yaw = static_cast<float>(util::normalizeAngleDegrees(yaw));
    pitch = static_cast<float>(util::normalizeAngleDegrees(pitch));
  }
};

inline std::ostream& operator<<(std::ostream& stream, const PlayerLook& obj)
{
  stream << boost::format("(%1%, %2%)") % obj.yaw % obj.pitch;
  return stream;
}

struct PlayerPosition : Vector3d, PlayerLook
{
  bool onGround;
  Dimension dimension;
  
  PlayerPosition(double x = 0.0, double y = 0.0, double z = 0.0, float yaw = 0.0f, float pitch = 0.0f, bool onGround = true, Dimension dimension = Dimension::Overworld)
        : Vector3d(x, y, z), PlayerLook(yaw, pitch), onGround(onGround), dimension(dimension) {}
  
  Vector2i getChunkPosition() const
  {
    return Vector2i(static_cast<std::int32_t>(x / 16), static_cast<std::int32_t>(z / 16));
  }
};

} // namespace redi


#endif // REDI_PLAYERPOSITION_HPP
