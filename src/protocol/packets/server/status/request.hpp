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
};

} // namespace redi

#endif // REDI_REQUEST_HPP