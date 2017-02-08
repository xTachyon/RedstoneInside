#include "unloadchunk.hpp"
#include "../../../packetwriternocopy.hpp"
#include "../../../../logger.hpp"

namespace redi
{
namespace packets
{

UnloadChunk::UnloadChunk(Vector2i position) : position(position) {}

void UnloadChunk::write(ByteBuffer& buffer)
{
  PacketWriterNoCopy packet(buffer, SendID);
  
  packet.writeInt(position.x);
  packet.writeInt(position.z);
  Logger::debug((boost::format("Send unload chunk %1%") % position).str());
}
  
} // namespace packets
} // namespace redi