#include <cstdint>
#include "protocol1_11.hpp"
#include "../bytebuffer.hpp"

namespace redi
{


void Protocol1_11::readHandshake(const ByteBuffer& buffer, std::size_t& offset)
{
  
}

void Protocol1_11::readPacket(const ByteBuffer& buffer)
{
  std::int32_t type;
//  protocol::Packet pkt;
}
} // namespace redi