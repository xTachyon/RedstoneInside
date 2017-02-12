#include "playerpositionandlook.hpp"
#include "../../../packetwriter.hpp"
#include "../../packethandler.hpp"

namespace redi
{
namespace packets
{

PlayerPositionAndLook::PlayerPositionAndLook(PacketReader& packet)
{
  read(packet);
}

PlayerPositionAndLook::PlayerPositionAndLook(double x, double y, double z, float yaw, float pitch, bool onGround, std::int32_t teleportID)
  : x(x), y(y), z(z), yaw(yaw), pitch(pitch), onGround(onGround), teleportID(teleportID) {}

PlayerPositionAndLook::PlayerPositionAndLook(redi::PlayerPosition position, std::int32_t teleportID)
  : PlayerPositionAndLook(position.x, position.y, position.z, position.yaw, position.pitch, position.onGround, teleportID) {}

void PlayerPositionAndLook::read(PacketReader& packet)
{
  x = packet.readDouble();
  y = packet.readDouble();
  z = packet.readDouble();
  yaw = packet.readFloat();
  pitch = packet.readFloat();
  onGround = packet.readBool();
}

void PlayerPositionAndLook::write(ByteBuffer& buffer)
{
  PacketWriter packet(buffer, SendID);
  
  packet.writeDouble(x);
  packet.writeDouble(y);
  packet.writeDouble(z);
  packet.writeFloat(yaw);
  packet.writeFloat(pitch);
  packet.writeByte(0);
  packet.writeVarInt(teleportID);
}

void PlayerPositionAndLook::process(PacketHandler& handler)
{
  handler.handlePlayerPositionAndLook(*this);
}
  
} // namespace packets
} // namespace redi