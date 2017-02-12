#include "loginsucces.hpp"
#include "../../../packetwriter.hpp"

namespace redi
{

LoginSucces::LoginSucces(const std::string& uuid, const std::string& username)
  : uuid(uuid), username(username) {}

LoginSucces::LoginSucces(std::string&& uuid, std::string&& username)
  : uuid(std::move(uuid)), username(std::move(username)) {}

void LoginSucces::write(ByteBuffer& buffer)
{
  PacketWriter packet(buffer, SendID);
  
  packet.writeString(uuid);
  packet.writeString(username);
}
  
} // namespace redi