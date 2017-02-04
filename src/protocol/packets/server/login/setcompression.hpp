#ifndef REDI_SETCOMPRESSION_HPP
#define REDI_SETCOMPRESSION_HPP

#include "../../packet.hpp"

namespace redi
{

struct SetCompression : public Packet
{
  static constexpr std::int32_t SendID = 0x03;
  
  std::int32_t threshold;
  
  SetCompression() = default;
  SetCompression(std::int32_t threshold);
  
  void write(ByteBuffer& buffer) override;
  
  const char* getName() const { return REDI_FUNCTION; }
};
  
} // namespace redi

#endif // REDI_SETCOMPRESSION_HPP