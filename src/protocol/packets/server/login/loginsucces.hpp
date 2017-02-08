#ifndef REDI_LOGINSUCCES_HPP
#define REDI_LOGINSUCCES_HPP

#include "../../packet.hpp"

namespace redi
{

struct LoginSucces : public Packet
{
  static constexpr std::int32_t SendID = 0x02;
  
  std::string uuid;
  std::string username;
  
  LoginSucces() = default;
  LoginSucces(const std::string& uuid, const std::string& username);
  LoginSucces(std::string&& uuid, std::string&& username);
  
  void write(ByteBuffer& buffer) override;
};
  
} // namespace redi

#endif // REDI_LOGINSUCCES_HPP