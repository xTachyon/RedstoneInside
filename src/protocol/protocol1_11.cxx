#include <cstdint>
#include <boost/format.hpp>
#include "protocol1_11.hpp"
#include "../bytebuffer.hpp"
#include "../session.hpp"
#include "packetreader.hpp"
#include "../logger.hpp"
#include "packetwriter.hpp"

namespace redi
{

void Protocol1_11::handlePacket(ByteBuffer& buffer)
{
  PacketReader reader(std::move(buffer));
  std::int32_t type = reader.readVarInt();
  
  switch (mSession->stage)
  {
    case State::Login:
    {
      switch (type)
      {
        case 0x00:
          handleHandshake(reader);
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
  }
}

void Protocol1_11::handleHandshake(PacketReader& reader)
{
  std::int32_t protocol = reader.readVarInt();
  std::string host = reader.readString();
  std::uint16_t port = reader.readUShort();
  std::int32_t nextstate = reader.readVarInt();
  
  Logger::info((boost::format("%1%:%2% with protocol version %3% just sent a handshake with next state %4%") % host % port % protocol % nextstate).str());
  
  switch (nextstate)
  {
    case 1:
      mSession->stage = State::Status;
      break;
    
    case 2:
      mSession->stage = State::Login;
      break;
    
    default:
      Logger::info("invalid state: " + std::to_string(nextstate));
  }
}

void Protocol1_11::handleStatusRequest(PacketReader&)
{
  PacketWriter writer(0x00);
  writer.writeString("{\"description\":{\"text\":\"REDI\"},\"players\":{\"max\":0,\"online\":0},\"version\":{\"name\":\"REDI\",\"protocol\":316}}");
  writer.finish();
  
  mSession->sendPacket(writer);
}

void Protocol1_11::handleStatusPing(PacketReader& reader)
{
  sendStatusPong(reader.readLong());
}

void Protocol1_11::sendStatusPong(std::int64_t number)
{
  PacketWriter writer(0x01);
  writer.writeLong(number);
  writer.finish();
  
  mSession->sendPacket(writer);
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  mSession->kill();
}
  
} // namespace redi