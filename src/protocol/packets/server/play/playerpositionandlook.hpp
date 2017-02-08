#ifndef REDI_PACKET_PLAYERPOSITIONANDLOOK_HPP
#define REDI_PACKET_PLAYERPOSITIONANDLOOK_HPP

#include "../../packet.hpp"
#include "../../../../playerposition.hpp"

namespace redi
{
namespace packets
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
  
  PlayerPositionAndLook(PacketReader& packet);
  PlayerPositionAndLook(double x = 0.0, double y = 0.0, double z = 0.0, float yaw = 0.0f, float pitch = 0.0f,
                        bool onGround = true, std::int32_t teleportID = 0);
  
  PlayerPositionAndLook(redi::PlayerPosition position, std::int32_t teleportID);
  
  void read(PacketReader& packet) override;
  void write(ByteBuffer& buffer) override;
  void process(PacketHandler& handler) override;
};
  
} // namespace packets
} // namespace redi

#endif // REDI_PACKET_PLAYERPOSITIONANDLOOK_HPP