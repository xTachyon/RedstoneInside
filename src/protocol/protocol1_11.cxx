#include <cstdint>
#include <boost/format.hpp>
#include <json.hpp>
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
  
//  Logger::info((boost::format("Packet with type %1% on state %2% from %3%") % type % getStateName(mSession->state) % getIP()).str());
  
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
  
  Logger::info((boost::format("Handhake with next state %1% from %2%") % getStateName(static_cast<ConnectionState>(nextstate)) % getIP()).str());
  
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
  Logger::info("Status ping from " + getIP());
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
  
  Logger::info((boost::format("Received Login start packet: %1%") % nick).str());
  
  mSession.setConnectionState(ConnectionState::Play);
  mSession.getServer().addPlayer(nick, &mSession);
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

void Protocol1_11::sendPlayerPositionAndLook()
{
  PacketWriter writer(0x2E);
  writer.writeDouble(0);
  writer.writeDouble(70);
  writer.writeDouble(0);
  writer.writeFloat(0);
  writer.writeFloat(0);
  writer.writeByte(0);
  writer.writeVarInt(6543);
  writer.commit();
  
  mSession.sendPacket(writer, "Send Player Position And Look");
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

ByteBuffer Protocol1_11::createChatPacket(const std::string& json, ChatPosition position)
{
  PacketWriter pkt(0x0F);
  
  pkt.writeString(json);
  pkt.writeByte(static_cast<std::int8_t>(position));
  
  pkt.commit();
  
  return pkt;
}
  
} // namespace redi