#include <cstdint>
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "protocol1_11.hpp"
#include "../bytebuffer.hpp"
#include "../session.hpp"
#include "packetreader.hpp"
#include "../logger.hpp"
#include "packetwriter.hpp"
#include "../server.hpp"

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
  writer.writeString("{\"description\":{\"text\":\"REDI\"},\"players\":{\"max\":0,\"online\":0},\"version\":{\"name\":\"REDI\",\"protocol\":316}}");
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
  std::string uuid = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
  
  Logger::info((boost::format("Received Login start packet: %1%") % nick).str());
  
  mSession->getServer().addPlayer(nick, uuid, mSession);
  sendLoginSucces(nick, uuid);
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
  
  
} // namespace redi