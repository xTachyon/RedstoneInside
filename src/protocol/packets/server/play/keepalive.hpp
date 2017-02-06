#ifndef REDI_KEEPALIVE_HPP
#define REDI_KEEPALIVE_HPP

#include "../../packet.hpp"

namespace redi
{
namespace packets
{

struct KeepAlive : public Packet
{
  static constexpr std::int32_t SendID = 0x1F;
  
  std::int32_t keepAliveID;
  
  KeepAlive(std::int32_t keepAliveID);
  
  void write(ByteBuffer& buffer) override;
  
  const char* getName() const override { return REDI_FUNCTION; }
};
  
} // namespace packets
} // namespace redi

#endif // REDI_KEEPALIVE_HPP