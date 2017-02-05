#ifndef REDI_PACKET_PLAYERPOSITIONANDLOOK_HPP
#define REDI_PACKET_PLAYERPOSITIONANDLOOK_HPP

#include "../../packet.hpp"
#include "../../../../playerposition.hpp"

namespace redi
{

struct PlayerPositionAndLook : public Packet
{
  static constexpr std::int32_t SendID = 0x2E;
  
  double x;
  double y;
  double z;
  float yaw;
  float pitch;
  bool onGround;
  std::int32_t teleportID;
  
  PlayerPositionAndLook(double x = 0.0, double y = 0.0, double z = 0.0, float yaw = 0.0f, float pitch = 0.0f, bool onGround = true, std::int32_t teleportID = 0);
  PlayerPositionAndLook(PlayerPosition position, std::int32_t teleportID);
  
  void write(ByteBuffer& buffer) override;
  
  const char* getName() const override { return REDI_FUNCTION; }
};
  
} // namespace redi

#endif // REDI_PACKET_PLAYERPOSITIONANDLOOK_HPP