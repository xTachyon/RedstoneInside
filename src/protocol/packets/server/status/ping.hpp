#pragma once

#include "../../packet.hpp"

namespace redi {

struct Ping : public Packet {
  std::int64_t payload;

  Ping(std::int64_t payload);
  Ping(PacketReader& packet);

  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
};

} // namespace redi