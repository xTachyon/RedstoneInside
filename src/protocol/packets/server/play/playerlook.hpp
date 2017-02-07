#ifndef REDI_PACKETS_PLAYERLOOK_HPP
#define REDI_PACKETS_PLAYERLOOK_HPP

#include "../../packet.hpp"
#include "../../../../playerposition.hpp"

namespace redi
{
namespace packets
{

struct PlayerLook : public Packet
{
  float yaw;
  float pitch;
  bool onGround;
  
  PlayerLook(PacketReader& packet);
  PlayerLook(float yaw = 0.0f, float pitch = 0.0f, bool onGround = true);
  
  void read(PacketReader& packet) override;
  void process(PacketHandler& handler) override;
};
  
} // namespace packets
  
} // namespace redi

#endif // REDI_PACKETS_PLAYERLOOK_HPP