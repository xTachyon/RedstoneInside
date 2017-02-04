#include "setcompression.hpp"
#include "../../../packetwriternocopy.hpp"

namespace redi
{

SetCompression::SetCompression(std::int32_t threshold) : threshold(threshold) {}

void SetCompression::write(ByteBuffer& buffer)
{
  PacketWriterNoCopy packet(buffer, SendID);
  
  packet.writeVarInt(threshold);
}
  
} // namespace redi