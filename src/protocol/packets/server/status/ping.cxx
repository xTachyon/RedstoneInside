#include "../../packethandler.hpp"
#include "ping.hpp"

namespace redi {

Ping::Ping(std::int64_t payload) : payload(payload) {}

Ping::Ping(PacketReader& packet) { read(packet); }

void Ping::read(PacketReader& packet) { payload = packet.readLong(); }

void Ping::process(PacketHandler& handler) { handler.handleStatusPing(*this); }

} // namespace redi