#ifndef REDI_HANDSHAKE_HPP
#define REDI_HANDSHAKE_HPP

#include "../../packet.hpp"
#include "../../../../enums.hpp"

namespace redi {

struct Handshake : public Packet {
  std::int32_t version;
  std::string hostname;
  std::uint16_t port;
  ConnectionState state;

  Handshake() = default;
  Handshake(PacketReader& packet);
  Handshake(std::int32_t version, std::string hostname, std::uint16_t port,
            ConnectionState state);

  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
};

} // namespace redi

#endif // REDI_HANDSHAKE_HPP