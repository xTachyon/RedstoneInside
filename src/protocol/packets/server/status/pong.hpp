#pragma once

#include "../../packet.hpp"

namespace redi {

struct Pong : public Packet {
  static constexpr std::int32_t SendID = 0x01;

  std::int64_t payload;

  Pong(std::int64_t payload);

  void write(ByteBuffer& buffer) override;
};

} // namespace redi