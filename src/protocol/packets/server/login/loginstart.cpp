#include "../../packethandler.hpp"
#include "loginstart.hpp"

namespace redi {

LoginStart::LoginStart(PacketReader& packet) { read(packet); }

LoginStart::LoginStart(const std::string& username) : username(username) {}

LoginStart::LoginStart(std::string&& username)
    : username(std::move(username)) {}

void LoginStart::read(PacketReader& packet) { username = packet.readString(); }

void LoginStart::process(PacketHandler& handler) {
  handler.handleLoginStart(*this);
}

} // namespace redi