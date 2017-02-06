#include "playerpositionandlook.hpp"
#include "../../../packetwriternocopy.hpp"

namespace redi
{

PlayerPositionAndLook::PlayerPositionAndLook(double x, double y, double z, float yaw, float pitch, bool onGround, std::int32_t teleportID)
  : x(x), y(y), z(z), yaw(yaw), pitch(pitch), onGround(onGround), teleportID(teleportID) {}

PlayerPositionAndLook::PlayerPositionAndLook(PlayerPosition position, std::int32_t teleportID)
  : PlayerPositionAndLook(position.x, position.y, position.z, position.yaw, position.pitch, position.onGround, teleportID) {}


void PlayerPositionAndLook::write(ByteBuffer& buffer)
{
  PacketWriterNoCopy packet(buffer, SendID);
  
  packet.writeDouble(x);
  packet.writeDouble(y);
  packet.writeDouble(z);
  packet.writeFloat(yaw);
  packet.writeFloat(pitch);
  packet.writeByte(0);
  packet.writeVarInt(teleportID);
}
  
} // namespace redi