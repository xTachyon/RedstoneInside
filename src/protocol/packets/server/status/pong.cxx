#include "pong.hpp"
#include "../../../packetwriternocopy.hpp"

namespace redi
{

Pong::Pong(std::int64_t payload) : payload(payload) {}

void Pong::write(ByteBuffer& buffer)
{
  PacketWriterNoCopy packet(buffer, SendID);
  
  packet.writeLong(payload);
}
  
} // namespace redi