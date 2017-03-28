#include <boost/format.hpp>
#include "joingame.hpp"
#include "../../../packetwriter.hpp"
#include "../../../../player.hpp"
#include "../../../../server.hpp"

namespace redi {

JoinGame::JoinGame(Player* ptr) : player(ptr) {}

void JoinGame::write(ByteBuffer& buffer) {
  using namespace std::string_literals;
  if (!player)
    throw std::runtime_error("Player can't be null - "s + REDI_FUNCTION);

  PacketWriter writer(buffer, SendID);

  writer.writeInt(player->getEntityID());
  writer.writeUByte(static_cast<std::uint8_t>(player->getGamemode()));
  writer.writeInt(static_cast<std::int32_t>(player->getDimension()));
  writer.writeUByte(
      static_cast<std::uint8_t>(player->getServer().config.difficulty));
  writer.writeUByte(
      static_cast<std::uint8_t>(player->getServer().config.maxPlayers));
  writer.writeString(player->getServer().config.levelType);
  writer.writeBool(player->getServer().config.reducedDebugInfo);
}

} // namespace redi