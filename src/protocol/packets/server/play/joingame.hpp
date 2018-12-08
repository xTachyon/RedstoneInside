#pragma once

#include "../../packet.hpp"
#include "../../../../player.hpp"

namespace redi {

struct JoinGame : public Packet {
  static constexpr std::int32_t SendID = 0x23;

  Player* player;

  JoinGame(Player* ptr = nullptr);

  void write(ByteBuffer& buffer) override;
};

} // namespace redi