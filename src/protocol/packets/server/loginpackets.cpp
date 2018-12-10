#include "loginpackets.hpp"
#include "../packethandler.hpp"
#include "../../packetwriter.hpp"

namespace redi::packets {

LoginStart::LoginStart(PacketReader& packet) { read(packet); }

LoginStart::LoginStart(const std::string& username) : username(username) {}

LoginStart::LoginStart(std::string&& username)
    : username(std::move(username)) {}

void LoginStart::read(PacketReader& packet) { username = packet.readString(); }

void LoginStart::process(PacketHandler& handler) {
  handler.handleLoginStart(*this);
}

LoginSucces::LoginSucces(const std::string& uuid, const std::string& username)
    : uuid(uuid), username(username) {}

LoginSucces::LoginSucces(std::string&& uuid, std::string&& username)
    : uuid(std::move(uuid)), username(std::move(username)) {}

void LoginSucces::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeString(uuid);
  packet.writeString(username);
}

SetCompression::SetCompression(std::int32_t threshold) : threshold(threshold) {}

void SetCompression::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeVarInt(threshold);
}

}