#include "disconnect.hpp"
#include "../../../packetwriternocopy.hpp"

namespace redi
{
namespace packets
{

Disconnect::Disconnect(const std::string& json, bool play) : json(json), play(play) {}

Disconnect::Disconnect(std::string&& json, bool play) : json(std::move(json)), play(play) {}

void Disconnect::write(ByteBuffer& buffer)
{
  PacketWriterNoCopy packet(buffer, play ? SendIDplay : SendIDlogin);
  
  packet.writeString(json);
}

} // namespace packets
} // namespace redi