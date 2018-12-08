#pragma once

#include "../../packet.hpp"
#include "../../../../player.hpp"

namespace redi {
namespace packets {

struct PlayerListItem : public Packet {
  static constexpr std::int32_t SendID = 0x2D;

  Player& player;
  PlayerListItemAction action;

  PlayerListItem(Player& player, PlayerListItemAction action);

  void write(ByteBuffer& buffer) override;
};

} // namespace packets
} // namespace redi