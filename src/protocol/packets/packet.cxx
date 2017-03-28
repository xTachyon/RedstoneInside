#include "../../session.hpp"
#include "../../player.hpp"
#include "packet.hpp"

namespace redi {

Packet::~Packet() {}

void Packet::send(Session& session) {
  ByteBuffer buffer;
  write(buffer);
  session.sendPacket(std::move(buffer), getName());
}

void Packet::send(Player& player) { send(player.getSession()); }

void Packet::send(SessionSharedPtr& session) { send(*session); }

} // namespace redi