#ifndef REDI_RESPONSE_HPP
#define REDI_RESPONSE_HPP

#include "../../packet.hpp"

namespace redi
{

class Server;

struct Response : public Packet
{
  static constexpr std::int32_t SendID = 0x00;

  Server& server;
  
  Response() = default;
  Response(Server& server);
  
  void write(ByteBuffer& buffer) override;
  
  const char* getName() const override { return REDI_FUNCTION; }
};

} // namespace redi

#endif // REDI_RESPONSE_HPP