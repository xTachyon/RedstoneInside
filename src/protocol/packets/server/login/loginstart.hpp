#ifndef REDI_LOGINSTART_HPP
#define REDI_LOGINSTART_HPP

#include "../../packet.hpp"

namespace redi
{

struct LoginStart : public Packet
{
  std::string username;
  
  LoginStart() = default;
  LoginStart(const std::string& username);
  LoginStart(std::string&& username);
  
  void read(const ByteBuffer& buffer) override;
};
  
} // namespace redi

#endif // REDI_LOGINSTART_HPP