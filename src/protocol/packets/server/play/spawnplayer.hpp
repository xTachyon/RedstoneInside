#pragma once

#include "../../packet.hpp"

namespace redi {
namespace packets {

struct SpawnPlayer : public Packet {
  static constexpr std::int32_t SendID = 0x05;

  Player& player;

  SpawnPlayer(Player& player);

  void write(ByteBuffer& buffer) override;
};

} // namespace packets
} // namespace redi