#include "entitylookandrelativemove.hpp"
#include "../../../packetwriter.hpp"

namespace redi
{
namespace packets
{

EntityLookAndRelativeMove::EntityLookAndRelativeMove(std::int32_t EID, std::int16_t deltax, std::int16_t deltay,
                                                     std::int16_t deltaz,
                                                     float yaw, float pitch, bool onGround)
      : EID(EID), deltax(deltax), deltay(deltay), deltaz(deltaz), yaw(yaw), pitch(pitch), onGround(onGround) {}

void EntityLookAndRelativeMove::write(ByteBuffer& buffer)
{
  PacketWriter packet(buffer, SendID);
  
  packet.writeVarInt(EID);
  packet.writeShort(deltax);
  packet.writeShort(deltay);
  packet.writeShort(deltaz);
  packet.writeAngle(yaw);
  packet.writeAngle(pitch);
  packet.writeBool(onGround);
}
  
} // namespace packets
} // namespace redi