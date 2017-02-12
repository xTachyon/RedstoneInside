#include "pong.hpp"
#include "../../../packetwriter.hpp"

namespace redi
{

Pong::Pong(std::int64_t payload) : payload(payload) {}

void Pong::write(ByteBuffer& buffer)
{
  PacketWriter packet(buffer, SendID);
  
  packet.writeLong(payload);
}
  
} // namespace redi