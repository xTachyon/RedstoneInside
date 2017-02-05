#ifndef REDI_REQUEST_HPP
#define REDI_REQUEST_HPP

#include "../../packet.hpp"

namespace redi
{

struct Request : public Packet
{
  Request() = default;
  Request(PacketReader& packet);
  
  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
  
  const char* getName() const override { return REDI_FUNCTION; }
};

} // namespace redi

#endif // REDI_REQUEST_HPP