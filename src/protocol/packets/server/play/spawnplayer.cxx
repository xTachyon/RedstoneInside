#include "spawnplayer.hpp"
#include "../../../../player.hpp"
#include "../../../packetwriternocopy.hpp"

namespace redi
{
namespace packets
{

SpawnPlayer::SpawnPlayer(Player& player) : player(player) {}

void SpawnPlayer::write(ByteBuffer& buffer)
{
  PacketWriterNoCopy packet(buffer, SendID);
  auto position = player.getPosition();
  
  packet.writeVarInt(player.getEntityID());
  packet.writeUUID(player.getUUID());
  packet.writeDouble(position.x);
  packet.writeDouble(position.y);
  packet.writeDouble(position.z);
  packet.writeAngle(position.yaw);
  packet.writeAngle(position.pitch);
  packet.writeUByte(0xFF);
}
  
} // namespace packets
} // namespace redi