#ifndef REDI_PING_HPP
#define REDI_PING_HPP

#include "../../packet.hpp"

namespace redi
{

struct Ping : public Packet
{
  std::int64_t payload;
  
  Ping(std::int64_t payload);
  Ping(PacketReader& packet);
  
  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
  
  const char* getName() const override { return REDI_FUNCTION; }
};
  
} // namespace redi

#endif // REDI_PING_HPP