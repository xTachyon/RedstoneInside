#ifndef REDI_PACKET
#define REDI_PACKET

# include <string>
#include <cstdint>
# include "../bytebuffer.hpp"

namespace redi
{
namespace protocol
{

enum class HandshakePacketType
{
  // um.. nothing ?
};

enum class PlayPacketType
{
  TabComplete = 14
};

enum class StatusPacketType
{
  Response = 0,
  Pong = 1,
};

class Packet
{
  public:
  
  ByteBuffer data;
  std::size_t readIndex;
  std::size_t startsAt;
  
  Packet(std::int32_t packetID);
  
  Packet(HandshakePacketType t);
  
  Packet(PlayPacketType t);
  
  Packet(StatusPacketType t);
  
  template <typename T, std::size_t size = sizeof(T)>
  void writeRaw(const T& ref)
  {
    data.append(reinterpret_cast<const std::uint8_t*>(&ref), size);
  }
  
  void writeBool(bool value);
  
  void writeSignedByte(std::int8_t value);
  
  void writeUnsignedByte(std::uint8_t value);
  
  void writeBigShort(std::int16_t value);
  
  void writeBigUnsignedShort(std::uint16_t value);
  
  void writeBigInt(std::int32_t value);
  
  void writeBigLong(std::uint64_t value);
  
  void writeBigFloat(float value);
  
  void writeBigDouble(double value);
  
  void writeVarInt(std::int32_t value);
  
  void writeVarLong(std::int64_t value);
  
  void writeString(const std::string& value);
  
  bool readBool();
  
  std::int8_t readSygnedByte();
  
  std::uint8_t readUnsignedByte();
  
  std::int16_t readBigShort();
  
  std::uint16_t readBigUnsignedShort();
  
  std::int32_t readBigInt();
  
  std::int64_t readBigLong();
  
  float readBigFloat();
  
  double readBigDouble();
  
  std::int32_t readVarInt();
  
  std::int64_t readVarLong();
  
  std::string readString();
  
  void finish();
  
  private:
  
  static constexpr std::int32_t fiveBytesInt = 1 << 31;
  
  ByteBuffer writeVarIntInternal(std::int64_t data);
  
  std::int64_t readVarIntInternal();
  
  void need(std::size_t bytes);
};
  
} // namespace protocol
} // namespace redi

#endif