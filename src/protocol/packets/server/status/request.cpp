#include "../../packethandler.hpp"
#include "request.hpp"

namespace redi {

Request::Request(PacketReader& packet) { read(packet); }

void Request::read(PacketReader&) {}

void Request::process(PacketHandler& handler) {
  handler.handleStatusRequest(*this);
}

} // namespace redi