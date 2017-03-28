#include "playerposition.hpp"
#include "../../../packetwriter.hpp"
#include "../../packethandler.hpp"

namespace redi {
namespace packets {

PlayerPosition::PlayerPosition(PacketReader& packet) { read(packet); }

PlayerPosition::PlayerPosition(double x, double y, double z, bool onGround)
    : x(x), y(y), z(z), onGround(onGround) {}

void PlayerPosition::read(PacketReader& packet) {
  x = packet.readDouble();
  y = packet.readDouble();
  z = packet.readDouble();
  onGround = packet.readBool();
}

void PlayerPosition::process(PacketHandler& handler) {
  handler.handlePlayerPosition(*this);
}

} // namespace packets
} // namespace redi