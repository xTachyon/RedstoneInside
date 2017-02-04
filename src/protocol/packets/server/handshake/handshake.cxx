#include "handshake.hpp"
#include "../../../../logger.hpp"
#include "../../../packetwriternocopy.hpp"

namespace redi
{


Handshake::Handshake(std::int32_t version, std::string hostname, std::uint16_t port, ConnectionState state)
  : version(version), hostname(hostname), port(port), state(state) {}

void Handshake::read(const ByteBuffer& buffer)
{
  PacketReader pkt(buffer);
  
  version = pkt.readVarInt();
  hostname = pkt.readString();
  port = pkt.readUShort();
  std::int32_t nextstate = pkt.readVarInt();
  
  switch (nextstate)
  {
  case 1:
    state = ConnectionState::Status;
    break;
  
  case 2:
    state = ConnectionState::Login;
    break;
  
  default:
    Logger::info(std::string("Invalid state: ") + std::to_string(nextstate));
  }
}
  
} // namespace redi