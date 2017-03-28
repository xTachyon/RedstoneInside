#ifndef REDI_LOGINSTART_HPP
#define REDI_LOGINSTART_HPP

#include "../../packet.hpp"

namespace redi {

struct LoginStart : public Packet {
  std::string username;

  LoginStart() = default;
  LoginStart(PacketReader& packet);
  LoginStart(const std::string& username);
  LoginStart(std::string&& username);

  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
};

} // namespace redi

#endif // REDI_LOGINSTART_HPP