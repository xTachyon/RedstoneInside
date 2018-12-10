#include "playpackets.hpp"
#include "../../chunkserializer13.hpp"
#include "../../../player.hpp"
#include "../../../chat/chatcomponent.hpp"
#include "../../../server.hpp"

namespace redi::packets {

ChatMessage::ChatMessage(const std::string& message, ChatPosition position)
    : message(message), position(position) {}

ChatMessage::ChatMessage(std::string&& message, ChatPosition position)
    : message(std::move(message)), position(position) {}

ChatMessage::ChatMessage(PacketReader& packet) { read(packet); }

void ChatMessage::read(PacketReader& packet) { message = packet.readString(); }

void ChatMessage::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeChat(message);
  packet.writeByte(static_cast<std::int8_t>(position));
}

void ChatMessage::process(PacketHandler& handler) {
  handler.handleChatMessage(*this);
}

ChunkData::ChunkData(const Chunk& chunk, Vector2i position)
    : chunk(chunk), position(position) {}

void ChunkData::write(ByteBuffer& buffer) {
  buffer = ChunkSerializer13(chunk, position)();
}

Disconnect::Disconnect(const std::string& json, bool play)
    : json(json), play(play) {}

Disconnect::Disconnect(std::string&& json, bool play)
    : json(std::move(json)), play(play) {}

void Disconnect::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, play ? SendIDplay : SendIDlogin);

  packet.writeChat(json);
}

EntityLookAndRelativeMove::EntityLookAndRelativeMove(
    std::int32_t EID, std::int16_t deltax, std::int16_t deltay,
    std::int16_t deltaz, float yaw, float pitch, bool onGround)
    : EID(EID), deltax(deltax), deltay(deltay), deltaz(deltaz), yaw(yaw),
      pitch(pitch), onGround(onGround) {}

void EntityLookAndRelativeMove::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeVarInt(EID);
  packet.writeShort(deltax);
  packet.writeShort(deltay);
  packet.writeShort(deltaz);
  packet.writeAngle(yaw);
  packet.writeAngle(pitch);
  packet.writeBool(onGround);
}

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

KeepAlive::KeepAlive(std::int32_t keepAliveID) : keepAliveID(keepAliveID) {}

void KeepAlive::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeVarInt(keepAliveID);
}

PlayerListItem::PlayerListItem(Player& player, PlayerListItemAction action)
    : player(player), action(action) {}

void PlayerListItem::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeVarInt(action);
  packet.writeVarInt(1);
  packet.writeUUID(player.getUUID());

  switch (action) {
    case PlayerListItemAction::AddPlayer: {
      packet.writeString(player.getUsername()); // name

      packet.writeVarInt(0); // number of properties

      packet.writeVarInt(player.getGamemode()); // gamemode
      packet.writeVarInt(0);                    // ping
      packet.writeBool(false);                  // has display name
    } break;

    case PlayerListItemAction::UpdateGamemode: {
      packet.writeVarInt(player.getGamemode()); // gamemode
    } break;

    case PlayerListItemAction::UpdateLantecy: {
      packet.writeVarInt(0); // ping
    } break;

    case PlayerListItemAction::UpdateDisplayName: {
      packet.writeBool(false); // has display name
    } break;

    case PlayerListItemAction::RemovePlayer:
      break;
  }
}

PlayerLook::PlayerLook(PacketReader& packet) { read(packet); }

PlayerLook::PlayerLook(float yaw, float pitch, bool onGround)
    : yaw(yaw), pitch(pitch), onGround(onGround) {}

void PlayerLook::read(PacketReader& packet) {
  yaw = packet.readFloat();
  pitch = packet.readFloat();
  onGround = packet.readBool();
}

void PlayerLook::process(PacketHandler& handler) {
  handler.handlePlayerLook(*this);
}

PlayerPosition::PlayerPosition(PacketReader& packet) { read(packet); }

PlayerPosition::PlayerPosition(double x, double y, double z, bool onGround)
    : x(x), y(y), z(z), onGround(onGround) {}

void PlayerPosition::read(PacketReader& packet) {
  x = packet.readDouble();
  y = packet.readDouble();
  z = packet.readDouble();
  onGround = packet.readBool();
}

void PlayerPosition::process(PacketHandler& handler) {
  handler.handlePlayerPosition(*this);
}

PlayerPositionAndLook::PlayerPositionAndLook(PacketReader& packet) {
  read(packet);
}

PlayerPositionAndLook::PlayerPositionAndLook(double x, double y, double z,
                                             float yaw, float pitch,
                                             bool onGround,
                                             std::int32_t teleportID)
    : x(x), y(y), z(z), yaw(yaw), pitch(pitch), onGround(onGround),
      teleportID(teleportID) {}

PlayerPositionAndLook::PlayerPositionAndLook(redi::PlayerPosition position,
                                             std::int32_t teleportID)
    : PlayerPositionAndLook(position.x, position.y, position.z, position.yaw,
                            position.pitch, position.onGround, teleportID) {}

void PlayerPositionAndLook::read(PacketReader& packet) {
  x = packet.readDouble();
  y = packet.readDouble();
  z = packet.readDouble();
  yaw = packet.readFloat();
  pitch = packet.readFloat();
  onGround = packet.readBool();
}

void PlayerPositionAndLook::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeDouble(x);
  packet.writeDouble(y);
  packet.writeDouble(z);
  packet.writeFloat(yaw);
  packet.writeFloat(pitch);
  packet.writeByte(0);
  packet.writeVarInt(teleportID);
}

void PlayerPositionAndLook::process(PacketHandler& handler) {
  handler.handlePlayerPositionAndLook(*this);
}

SpawnPlayer::SpawnPlayer(Player& player) : player(player) {}

void SpawnPlayer::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);
  auto position = player.getPosition();

  packet.writeVarInt(player.getEntityID());
  packet.writeUUID(player.getUUID());
  packet.writeDouble(position.x);
  packet.writeDouble(position.y);
  packet.writeDouble(position.z);
  packet.writeAngle(position.yaw);
  packet.writeAngle(position.pitch);
  packet.writeUByte(0xFF);
}

SpawnPosition::SpawnPosition(Vector3i position) : position(position) {}

void SpawnPosition::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writePosition(position.x, position.y, position.z);
}

UnloadChunk::UnloadChunk(Vector2i position) : position(position) {}

void UnloadChunk::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeInt(position.x);
  packet.writeInt(position.z);
  //  Logger::debug((boost::format("Send unload chunk %1%") % position).str());
}

}