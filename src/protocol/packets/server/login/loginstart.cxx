#include "loginstart.hpp"

namespace redi
{

LoginStart::LoginStart(const std::string& username) : username(username) {}

LoginStart::LoginStart(std::string&& username) : username(std::move(username)) {}

void LoginStart::read(const ByteBuffer& buffer)
{
  PacketReader packet(buffer);
  
  username = packet.readString();
}
  
} // namespace redi