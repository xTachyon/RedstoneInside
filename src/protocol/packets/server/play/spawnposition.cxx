#include "spawnposition.hpp"
#include "../../../packetwriternocopy.hpp"

namespace redi
{

SpawnPosition::SpawnPosition(Vector3i position) : position(position) {}

void SpawnPosition::write(ByteBuffer& buffer)
{
  PacketWriterNoCopy packet(buffer, SendID);
  
  packet.writePosition(position.x, position.y, position.z);
}
  
} // namespace redi