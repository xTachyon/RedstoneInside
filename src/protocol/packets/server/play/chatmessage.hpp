#ifndef REDI_CHATMESSAGE_HPP
#define REDI_CHATMESSAGE_HPP

#include "../../packet.hpp"
#include "../../../../enums.hpp"

namespace redi
{
namespace packets
{
  
struct ChatMessage : public Packet
{
  static constexpr std::int32_t SendID = 0x0F;
  
  std::string message;
  ChatPosition position;
  
  ChatMessage(const std::string& message = "", ChatPosition position = ChatPosition::ChatBox);
  ChatMessage(std::string&& message, ChatPosition position);
  ChatMessage(PacketReader& packet);
  
  void read(PacketReader& packet) override;
  void write(ByteBuffer& buffer) override;
  virtual void process(PacketHandler& handler) override;
};

}
} // namespace redi

#endif // REDI_CHATMESSAGE_HPP