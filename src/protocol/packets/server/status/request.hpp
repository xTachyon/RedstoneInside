#pragma once

#include "../../packet.hpp"

namespace redi {

struct Request : public Packet {
  Request() = default;
  Request(PacketReader& packet);

  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
};

} // namespace redi