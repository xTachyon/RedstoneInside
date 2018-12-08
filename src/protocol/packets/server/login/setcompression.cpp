#include "setcompression.hpp"
#include "../../../packetwriter.hpp"

namespace redi {

SetCompression::SetCompression(std::int32_t threshold) : threshold(threshold) {}

void SetCompression::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeVarInt(threshold);
}

} // namespace redi