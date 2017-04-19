#include "disconnect.hpp"
#include "../../../packetwriter.hpp"
#include "../../../../chat/chatcomponent.hpp"

namespace redi {
namespace packets {

Disconnect::Disconnect(const std::string& json, bool play)
    : json(json), play(play) {}

Disconnect::Disconnect(std::string&& json, bool play)
    : json(std::move(json)), play(play) {}

void Disconnect::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, play ? SendIDplay : SendIDlogin);
  
  packet.writeChat(json);
}

} // namespace packets
} // namespace redi