#include "../../session.hpp"
#include "packet.hpp"

namespace redi
{

Packet::~Packet() {}

void Packet::send(Session& session)
{
  ByteBuffer buffer;
  write(buffer);
  session.sendPacket(std::move(buffer), getName());
}

} // namespace redi