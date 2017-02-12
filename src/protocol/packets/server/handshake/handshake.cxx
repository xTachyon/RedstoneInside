#include "handshake.hpp"
#include "../../../../logger.hpp"
#include "../../../packetwriter.hpp"
#include "../../packethandler.hpp"

namespace redi
{

Handshake::Handshake(PacketReader& packet)
{
  read(packet);
}

Handshake::Handshake(std::int32_t version, std::string hostname, std::uint16_t port, ConnectionState state)
  : version(version), hostname(hostname), port(port), state(state) {}

void Handshake::read(PacketReader& packet)
{
  version = packet.readVarInt();
  hostname = packet.readString();
  port = packet.readUShort();
  std::int32_t nextstate = packet.readVarInt();
  
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

void Handshake::process(PacketHandler& handler)
{
  handler.handleHandshake(*this);
}
  
} // namespace redi