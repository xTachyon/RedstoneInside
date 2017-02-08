#ifndef REDI_PONG_HPP
#define REDI_PONG_HPP

#include "../../packet.hpp"

namespace redi
{

struct Pong : public Packet
{
  static constexpr std::int32_t SendID = 0x01;
  
  std::int64_t payload;
  
  Pong(std::int64_t payload);
  
  void write(ByteBuffer& buffer) override;
};
  
} // namespace redi

#endif // REDI_PONG_HPP