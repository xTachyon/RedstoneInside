#include "keepalive.hpp"
#include "../../../packetwriter.hpp"

namespace redi {
namespace packets {

KeepAlive::KeepAlive(std::int32_t keepAliveID) : keepAliveID(keepAliveID) {}

void KeepAlive::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeVarInt(keepAliveID);
}

} // namespace packets
} // namespace redi