#include <cstdint>
#include <boost/format.hpp>
#include <json.hpp>
#include <unordered_set>
#include "protocol1_11.hpp"
#include "../bytebuffer.hpp"
#include "../session.hpp"
#include "packetreader.hpp"
#include "../logger.hpp"
#include "packetwriter.hpp"
#include "../server.hpp"
#include "../player.hpp"
#include "../util/util.hpp"
#include "chunkserializer13.hpp"

namespace redi
{

void Protocol1_11::handlePacket(ByteBuffer& buffer)
{
  PacketReader pkt;

//  if (mSession->mSetCompressionIsSent) pkt = PacketReader::getFromCompressedPacket(buffer);
//  else
  pkt.data = std::move(buffer);
  
  std::int32_t type = pkt.readVarInt();

  switch (mSession.getConnectionState())
  {
  case ConnectionState::Login:
  {
    switch (type)
    {
    case 0x00:
      handleLoginStart(pkt);
      break;
    
    default:
      break;
    }
  }
    break;
  
  case ConnectionState::Status:
  {
    switch (type)
    {
    case 0x00:
      handleStatusRequest(pkt);
      break;
    
    case 0x01:
      handleStatusPing(pkt);
      break;
    
    default:
      break;
    }
  }
    break;
  
  case ConnectionState::Play:
  {
    switch (type)
    {
    case 0x02:
      handleChatMessage(pkt);
      break;
    
    case 0x04:
      handleClientSettings(pkt);
      break;
    
    case 0x0C:
      handlePlayerPosition(pkt);
      break;
    
    case 0x0D:
      handlePlayerPositionAndLook(pkt);
      break;
    
    case 0x0E:
      handlePlayerLook(pkt);
      break;
    
    case 0x1C:
//        handlePlayerBlockPlacement(pkt);
      break;
    
    default:
      break;
    }
  }
    break;
  
  case ConnectionState::Handshake:
  {
    switch (type)
    {
    case 0x00:
      handleHandshake(pkt);
      break;
    
    default:
      break;
    }
  }
  }
}

void Protocol1_11::handleHandshake(PacketReader& reader)
{
  reader.consumeVarInt();
  reader.consumeString();
  reader.consumeUShort();
  std::int32_t nextstate = reader.readVarInt();
  
  switch (nextstate)
  {
  case 1:
    mSession.setConnectionState(ConnectionState::Status);
    break;
  
  case 2:
    mSession.setConnectionState(ConnectionState::Login);
    break;
  
  default:
    Logger::info(std::string("invalid state: ") + std::to_string(nextstate));
  }
}

void Protocol1_11::handleStatusRequest(PacketReader&)
{
  mSession.getServer().addEvent(std::make_shared<EventStatusRequest>(mSession));
}

void Protocol1_11::handleStatusPing(PacketReader& reader)
{
  sendStatusPong(reader.readLong());
}

void Protocol1_11::sendStatusPong(std::int64_t number)
{
  PacketWriter writer(0x01);
  writer.writeLong(number);
  writer.commit(false);
  
  mSession.sendPacket(writer, "Status Pong");
  mSession.disconnect();
}

std::string Protocol1_11::getIP(boost::asio::ip::tcp::socket& socket)
{
  try
  {
    return socket.remote_endpoint().address().to_string();
  }
  catch (std::exception&) {}
  
  return {};
}

std::string Protocol1_11::getIP()
{
  return getIP(mSession.getSocket());
}

void Protocol1_11::handleLoginStart(PacketReader& reader)
{
  std::string nick = reader.readString();
  
  mSession.setConnectionState(ConnectionState::Play);
  mSession.getServer().addEvent(std::make_shared<EventPlayerJoin>(mSession, std::move(nick)));
}

void Protocol1_11::sendLoginSucces(const std::string& nick, const std::string& uuid)
{
  PacketWriter writer(0x02);
  writer.writeString(uuid);
  writer.writeString(nick);
  writer.commit();
  
  mSession.sendPacket(writer, "Send Login Success");
}

void Protocol1_11::sendJoinGame(const Player& player)
{
  PacketWriter writer(0x23);
  writer.writeInt(player.getEntityID());
  writer.writeUByte(static_cast<std::uint8_t>(player.getGamemode()));
  writer.writeInt(static_cast<std::int32_t>(player.getDimension()));
  writer.writeUByte(static_cast<std::uint8_t>(player.getServer().config.difficulty));
  writer.writeUByte(static_cast<std::uint8_t>(player.getServer().config.maxPlayers));
  writer.writeString(player.getServer().config.levelType);
  writer.writeBool(player.getServer().config.reducedDebugInfo);
  writer.commit();
  
  mSession.sendPacket(writer, "Send Join Game");
}

void Protocol1_11::sendSetCompression()
{
  PacketWriter writer(0x03);
  writer.writeVarInt(-1);
  writer.commit(false);
  
  mSession.sendPacket(writer, "Sent set compression");
  mSession.setCompressionIsSent(true);
}

void Protocol1_11::sendSpawnPosition()
{
  PacketWriter writer(0x43);
  writer.writePosition(0, 100, 0);
  writer.commit();
  
  mSession.sendPacket(writer, "Sent Spawn Position");
}

void Protocol1_11::sendPlayerAbilities()
{
  PacketWriter writer(0x2B);
  writer.writeByte(PlayerAbilitiesFlag::AllowFlying | PlayerAbilitiesFlag::CreativeMode | PlayerAbilitiesFlag::Flying | PlayerAbilitiesFlag::Invulnerable);
  writer.writeFloat(1.0f);
  writer.writeFloat(1.0f);
  writer.commit();
  
  mSession.sendPacket(writer, "Send Player Abilities");
}

void Protocol1_11::handleClientSettings(PacketReader&)
{
//  std::string locale = reader.readString();
//  std::int8_t viewdistance = reader.readByte();
//  std::int32_t chatmode = reader.readVarInt();
//  bool chatcolors = reader.readBool();
//  std::uint8_t skinparts = reader.readUByte();
//  std::int32_t mainhand = reader.readVarInt();
}

void Protocol1_11::sendPlayerPositionAndLook(Player& player)
{
  PlayerPosition position = player.getPosition();
  PacketWriter pkt(0x2E);
  
  pkt.writeDouble(position.x);
  pkt.writeDouble(position.y);
  pkt.writeDouble(position.z);
  pkt.writeFloat(position.yaw);
  pkt.writeFloat(position.pitch);
  pkt.writeByte(0);
  pkt.writeVarInt(player.getNewTeleportID());
  pkt.commit();
  
  mSession.sendPacket(pkt, "Send Player Position And Look");
}

void Protocol1_11::sendKeepAlive()
{
  PacketWriter writer(0x1F);
  writer.writeVarInt(util::getRandomInt32());
  writer.commit();
  
  mSession.sendPacket(writer, "Send Keep Alive");
}

void Protocol1_11::sendTimeUpdate()
{
  PacketWriter writer(0x44);
  writer.writeLong(0);
  writer.writeLong(0);
  writer.commit();
  
  mSession.sendPacket(writer, "Send Time Update");
}

void Protocol1_11::sendChunk(const Chunk& chunk, Vector2i pos)
{
  mSession.sendPacket(ChunkSerializer13(chunk, pos)(), "Send Chunk data");
}

void Protocol1_11::handleChatMessage(PacketReader& pkt)
{
  mSession.getServer().addEvent(std::make_shared<EventChatMessage>(mSession.getPlayer(), pkt.readString()));
}

void Protocol1_11::handlePlayerLook(PacketReader& pkt)
{
  float yaw = pkt.readFloat();
  float pitch = pkt.readFloat();
  bool onGround = pkt.readBool();

  mSession.getServer().addEvent(std::make_shared<EventPlayerLook>(mSession.getPlayer(), yaw, pitch, onGround));
}

void Protocol1_11::handlePlayerPosition(PacketReader& pkt)
{
  double x = pkt.readDouble();
  double y = pkt.readDouble();
  double z = pkt.readDouble();
  bool onGround = pkt.readBool();

  mSession.getServer().addEvent(std::make_shared<EventPlayerPosition>(mSession.getPlayer(), x, y, z, onGround));
}

void Protocol1_11::handlePlayerPositionAndLook(PacketReader& pkt)
{
  double x = pkt.readDouble();
  double y = pkt.readDouble();
  double z = pkt.readDouble();
  float yaw = pkt.readFloat();
  float pitch = pkt.readFloat();
  bool onGround = pkt.readBool();

  mSession.getServer().addEvent(std::make_shared<EventPlayerPositionAndLook>(mSession.getPlayer(), x, y, z, yaw, pitch, onGround));
}

ByteBuffer Protocol1_11::createChatPacket(const std::string& json, ChatPosition position)
{
  PacketWriter pkt(0x0F);
  
  pkt.writeString(json);
  pkt.writeByte(static_cast<std::int8_t>(position));
  
  pkt.commit();
  
  return pkt;
}

ByteBuffer Protocol1_11::createPlayerListItemPacket(Player& player, PlayerListItemAction action)
{
  PacketWriter pkt(0x2D);
  
  pkt.writeVarInt(action); // action
  pkt.writeVarInt(1); // number of players
  pkt.writeUUID(player.getUUID()); // uuid
  
  switch (action)
  {
  case PlayerListItemAction::AddPlayer:
  {
    pkt.writeString(player.getUsername()); // name
    
    pkt.writeVarInt(1); // number of properties
    pkt.writeString("textures"); // name
    pkt.writeString("eyJ0aW1lc3RhbXAiOjE0ODU3MTY4Mzk2NTYsInByb2ZpbGVJZCI6ImIwZDRiMjhiYzFkNzQ4ODlhZjBlODY2MWNlZTk2YWFiIiwicHJvZmlsZU5hbWUiOiJZZWxlaGEiLCJzaWduYXR1cmVSZXF1aXJlZCI6dHJ1ZSwidGV4dHVyZXMiOnsiU0tJTiI6eyJ1cmwiOiJodHRwOi8vdGV4dHVyZXMubWluZWNyYWZ0Lm5ldC90ZXh0dXJlL2E4M2ZkNTI0MWJiYTlkMTMxMWU1NDEzMTUwZWY0MjQ1MjI2NWY0MzE2OGRlOGM0ZTE5NDY4ZWYzZjY5MzcyIn19fQ==");
    // value
    pkt.writeBool(true); // is signed
    pkt.writeString("xtzSRLv9iAbWig9GryoTOPp1Jxv2hjolffZaHp6hcX3b0/4SSvErZGOLcHzXIXwDT7f9jcdcEHKdB69WSIZkM3fIAQ8wVtww03KCpkgXkvqixz7MxOdYw0XtPeA0oPFxX3gvVNys/3zvKdgvFYkTh1zIo8D9qA+rP6A+x2ITJnXQNLh+ggHYELAw2QHQAn7x51ivsxs5Y/0Ji9/0wgDKn+DJ1DDzH+JgoRJNQkhhNkGgM/MpTEQZMabONGmwJG6AR0syKgDSVEbSORM1lDmsowbEQ8o3GN56leg3H7YIEFww00PehT5eLRNDRe8RXEykSLj6bs0PaqdUx+IZb7vzT14SJc0kn+h7MCWGHrr2qmldivtGG2alK++PYLyLQ5zs7c82pmwlLlNXZJa3+cbzSihrpywmaIntdITFo/Trtt+ZrvVBk56qihzz/POyow/Zj+Z7lAKv+vm/ZE8nfgLPOOIC7PAIj9/U5fwZTq4L8zXhb0ZRk5wrAJQ3iKDZD8u99t1CSDsUHHmTQmyzngk00Z/Ae5QMTGf2hEnLAcfhgJdS1NZLg1hsacNT8NsxrnQlD0KDLC9goO4QrtTnG50cnpJQggCOiFubMiJCmYq62fajmqVN99RrLN74CVxtLu9hTDeFontZdm2W3I0ks92SMrrEhQuNumcVhyRF4bAz6Qk=");
    
    pkt.writeVarInt(player.getGamemode()); // gamemode
    pkt.writeVarInt(0); // ping
    pkt.writeBool(false); // has display name
  }
    break;
  
  case PlayerListItemAction::UpdateGamemode:
  {
    pkt.writeVarInt(player.getGamemode()); // gamemode
  }
    break;
  
  case PlayerListItemAction::UpdateLantecy:
  {
    pkt.writeVarInt(0); // ping
  }
    break;
  
  case PlayerListItemAction::UpdateDisplayName:
  {
    pkt.writeBool(false); // has display name
  }
    break;
  
  case PlayerListItemAction::RemovePlayer:
    break;
  }
  
  pkt.commit();
  return pkt;
}

void Protocol1_11::sendPlayerListItem(Player& player, PlayerListItemAction action)
{
  ByteBufferSharedPtr buffer = std::make_shared<ByteBuffer>(createPlayerListItemPacket(player, action));
  mSession.sendPacket(buffer);
}

ByteBuffer Protocol1_11::createSpawnPlayerPacket(Player& player)
{
  PlayerPosition position = player.getPosition();
  PacketWriter pkt(0x05);
  
  pkt.writeVarInt(player.getEntityID());
  pkt.writeUUID(player.getUUID());
  pkt.writeDouble(position.x);
  pkt.writeDouble(position.y);
  pkt.writeDouble(position.z);
  pkt.writeFloat(position.yaw);
  pkt.writeFloat(position.pitch);
  pkt.writeUByte(0xFF);
  pkt.commit();
  
  return pkt;
}

void Protocol1_11::sendSpawnPlayerPacket(Player& player)
{
  ByteBufferSharedPtr buffer = std::make_shared<ByteBuffer>(createSpawnPlayerPacket(player));
  mSession.sendPacket(buffer);
}
  
} // namespace redi