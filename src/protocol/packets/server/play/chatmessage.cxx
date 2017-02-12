#include "chatmessage.hpp"
#include "../../../packetwriter.hpp"
#include "../../packethandler.hpp"

namespace redi
{
namespace packets
{

ChatMessage::ChatMessage(const std::string& message, ChatPosition position) : message(message), position(position) {}

ChatMessage::ChatMessage(std::string&& message, ChatPosition position): message(std::move(message)), position(position) {}

ChatMessage::ChatMessage(PacketReader& packet)
{
  read(packet);
}

void ChatMessage::read(PacketReader& packet)
{
  message = packet.readString();
}

void ChatMessage::write(ByteBuffer& buffer)
{
  PacketWriter packet(buffer, SendID);
  
  packet.writeString(message);
  packet.writeByte(static_cast<std::int8_t>(position));
}

void ChatMessage::process(PacketHandler& handler)
{
  handler.handleChatMessage(*this);
}
  
} // namspace packets
} // namespace redi