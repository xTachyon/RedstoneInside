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
  
  const ServerConfig& config = player->getServer().getServerConfiguration();
  PacketWriter writer(buffer, SendID);

  writer.writeInt(player->getEntityID());
  writer.writeUByte(static_cast<std::uint8_t>(player->getGamemode()));
  writer.writeInt(static_cast<std::int32_t>(player->getDimension()));
  writer.writeUByte(
      static_cast<std::uint8_t>(config.difficulty));
  writer.writeUByte(
      static_cast<std::uint8_t>(config.maxPlayers));
  writer.writeString(config.levelType);
  writer.writeBool(config.reducedDebugInfo);
}

} // namespace redi