#include "keepalive.hpp"
#include "../../../packetwriternocopy.hpp"

namespace redi
{
namespace packets
{

KeepAlive::KeepAlive(std::int32_t keepAliveID) : keepAliveID(keepAliveID) {}

void KeepAlive::write(ByteBuffer& buffer)
{
  PacketWriterNoCopy packet(buffer, SendID);
  
  packet.writeVarInt(keepAliveID);
}
  
} // namespace packets
} // namespace redi