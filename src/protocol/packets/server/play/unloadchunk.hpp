#pragma once

#include "../../packet.hpp"

namespace redi {
namespace packets {

struct UnloadChunk : public Packet {
  static constexpr std::int32_t SendID = 0x1D;

  Vector2i position;

  UnloadChunk(Vector2i position);

  void write(ByteBuffer& buffer) override;
};

} // namespace packets
} // namespace redi