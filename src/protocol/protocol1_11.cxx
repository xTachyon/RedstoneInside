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

namespace redi
{

void Protocol1_11::handlePacket(ByteBuffer& buffer)
{
  PacketReader reader;
  
//  if (mSession->setCompressionSent) reader = PacketReader::getFromCompressedPacket(buffer);
//  else
  reader.data = std::move(buffer);
  
  std::int32_t type = reader.readVarInt();
  
  Logger::info((boost::format("Packet with type %1% on state %2% from %3%") % type % getStateName(mSession->state) % getIP()).str());
  
  switch (mSession->state)
  {
    case State::Login:
    {
      switch (type)
      {
      case 0x00:
        handleLoginStart(reader);
        break;
      }
    }
      break;
    
    case State::Status:
    {
      switch (type)
      {
        case 0x00:
          handleStatusRequest(reader);
          break;
          
        case 0x01:
          handleStatusPing(reader);
          break;
      }
    }
      break;
      
    case State::Play:
    {
      switch (type)
      {
      case 0x04:
        handleClientSettings(reader);
      }
    }
    break;
    
  case State::Handshake:
  {
    switch (type)
    {
    case 0x00:
      handleHandshake(reader);
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
  
  Logger::info((boost::format("Handhake with next state %1% from %2%") % getStateName(static_cast<State>(nextstate)) % getIP()).str());
  
  switch (nextstate)
  {
    case 1:
      mSession->state = State::Status;
      break;
    
    case 2:
      mSession->state = State::Login;
      break;
    
    default:
      Logger::info(std::string("invalid state: ") + std::to_string(nextstate));
  }
}

void Protocol1_11::handleStatusRequest(PacketReader&)
{
  PacketWriter writer(0x00);
  
  const ServerConfig& config = mSession->getServer().config;
  nlohmann::json j;
  j["description"]["text"] = config.motd;
  j["version"]["name"] = "Redi";
  j["version"]["protocol"] = 316;
  j["players"]["max"] = config.maxPlayers;
  j["players"]["online"] = 0;
  
  writer.writeString(j.dump());
  writer.commit(false);
  
  mSession->sendPacket(writer, "Status Request");
  
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
  
  mSession->sendPacket(writer, "Status Pong");
  mSession->kill();
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
  return getIP(mSession->getSocket());
}

void Protocol1_11::handleLoginStart(PacketReader& reader)
{
  std::string nick = reader.readString();
  
  Logger::info((boost::format("Received Login start packet: %1%") % nick).str());
  
  mSession->state = State::Play;
  mSession->getServer().addPlayer(nick, mSession);
}

void Protocol1_11::sendLoginSucces(const std::string& nick, const std::string& uuid)
{
  PacketWriter writer(0x02);
  writer.writeString("ecc8e29d-0936-42cb-8492-14ed40555ffd");
  writer.writeString(nick);
  writer.commit();
 
  mSession->sendPacket(writer, "Send Login Success");
}

void Protocol1_11::sendJoinGame(const Player& player)
{
  PacketWriter writer(0x23);
  writer.writeInt(player.id);
  writer.writeUByte(static_cast<std::uint8_t>(player.getGamemode()));
  writer.writeInt(static_cast<std::int32_t>(player.getDimension()));
  writer.writeUByte(static_cast<std::uint8_t>(player.getServer().config.difficulty));
  writer.writeUByte(static_cast<std::uint8_t>(player.getServer().config.maxPlayers));
  writer.writeString(player.getServer().config.evelType);
  writer.writeBool(player.getServer().config.reducedDebugInfo);
  writer.commit();
  
  mSession->sendPacket(writer, "Send Join Game");
}

void Protocol1_11::sendSetCompression()
{
  PacketWriter writer(0x03);
  writer.writeVarInt(-1);
  writer.commit(false);
  
  mSession->sendPacket(writer, "Sent set compression");
  mSession->setCompressionSent = true;
}

void Protocol1_11::sendSpawnPosition()
{
  PacketWriter writer(0x43);
  writer.writePosition(0, 0, 0);
  writer.commit();
  
  mSession->sendPacket(writer, "Sent Spawn Position");
}

void Protocol1_11::sendPlayerAbilities()
{
  PacketWriter writer(0x2B);
  writer.writeByte(PlayerAbilitiesFlag::AllowFlying | PlayerAbilitiesFlag::CreativeMode | PlayerAbilitiesFlag::Flying | PlayerAbilitiesFlag::Invulnerable);
  writer.writeFloat(1.0f);
  writer.writeFloat(1.0f);
  writer.commit();

  mSession->sendPacket(writer, "Send Player Abilities");
}

void Protocol1_11::handleClientSettings(PacketReader& reader)
{
  std::string locale = reader.readString();
  std::int8_t viewdistance = reader.readByte();
  std::int32_t chatmode = reader.readVarInt();
  bool chatcolors = reader.readBool();
  std::uint8_t skinparts = reader.readUByte();
  std::int32_t mainhand = reader.readVarInt();
}

void Protocol1_11::sendPlayerPositionAndLook()
{
  PacketWriter writer(0x2E);
  writer.writeDouble(0);
  writer.writeDouble(0);
  writer.writeDouble(0);
  writer.writeFloat(0);
  writer.writeFloat(0);
  writer.writeByte(0);
  writer.writeVarInt(6543);
  writer.commit();
  
  mSession->sendPacket(writer, "Send Player Position And Look");
}

void Protocol1_11::sendKeepAlive()
{
  PacketWriter writer(0x1F);
  writer.writeVarInt(util::getRandomInt32());
  writer.commit();
  
  mSession->sendPacket(writer, "Send Keep Alive");
}

void Protocol1_11::sendTimeUpdate()
{
  PacketWriter writer(0x44);
  writer.writeLong(0);
  writer.writeLong(0);
  writer.commit();
  
  mSession->sendPacket(writer, "Send Time Update");
}

} // namespace redi