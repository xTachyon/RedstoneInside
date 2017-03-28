#ifndef REDI_PACKETS_ENTITYLOOKANDRELATIVEMOVE_HPP
#define REDI_PACKETS_ENTITYLOOKANDRELATIVEMOVE_HPP

#include "../../packet.hpp"

namespace redi {
namespace packets {

struct EntityLookAndRelativeMove : public Packet {
  static constexpr std::int32_t SendID = 0x26;

  std::int32_t EID;
  std::int16_t deltax;
  std::int16_t deltay;
  std::int16_t deltaz;
  float yaw;
  float pitch;
  bool onGround;

  EntityLookAndRelativeMove(std::int32_t EID = 0, std::int16_t deltax = 0,
                            std::int16_t deltay = 0, std::int16_t deltaz = 0,
                            float yaw = 0, float pitch = 0,
                            bool onGround = true);

  void write(ByteBuffer& buffer) override;
};

} // namespace packets
} // namespace redi

#endif // REDI_PACKETS_ENTITYLOOKANDRELATIVEMOVE_HPP