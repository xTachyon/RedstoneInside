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

namespace redi
{

void Protocol1_11::handlePacket(ByteBuffer& buffer)
{
  PacketReader reader(std::move(buffer));
  std::int32_t type = reader.readVarInt();
  
  Logger::info((boost::format("Packet with type %1% on state %2% from %3%") % type % getStateName(mSession->stage) % getIP()).str());
  
  switch (mSession->stage)
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
      mSession->stage = State::Status;
      break;
    
    case 2:
      mSession->stage = State::Login;
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
  writer.finish();
  
  Logger::info("Status request from " + getIP());
  
  mSession->sendPacket(writer);
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
  writer.finish();
  
  Logger::info("Status pong " + std::to_string(number) + " from " + getIP());
  
  mSession->sendPacket(writer);
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
  
  mSession->getServer().addPlayer(nick, mSession);
  mSession->stage = State::Play;
}

void Protocol1_11::sendLoginSucces(const std::string& nick, const std::string& uuid)
{
  PacketWriter writer(0x02);
  writer.writeString(uuid);
  writer.writeString(nick);
  writer.finish();
  mSession->sendPacket(writer);
  
  Logger::info((boost::format("Sending Login success packet: %1% -- %2%") % nick % uuid).str());
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
  writer.finish();
  mSession->sendPacket(writer);
  
  Logger::info("Sent join game packet");
}

void Protocol1_11::sendSetCompression()
{
  PacketWriter writer(0x03);
  writer.writeVarInt(60000);
  writer.finish();
  
  std::cout << "\n\n";
  for (std::size_t i = 0; i < writer.data.size(); ++i)
    std::cout << (int)writer.data[i] << ' ';
  std::cout << "\n\n";


//  PacketReader r(std::move(writer.data));
//  auto a = r.readVarInt();
//  auto b = r.readVarInt();
//  auto c = r.readVarInt();
  mSession->sendPacket(writer);
  
  Logger::info("Sent set compression");
}
  
  
} // namespace redi