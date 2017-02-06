#ifndef REDI_PACKETREADER_HPP
#define REDI_PACKETREADER_HPP

#include <string>
#include <boost/endian/conversion.hpp>
#include "../bytebuffer.hpp"
#include "../vectorn.hpp"

namespace redi
{

class PacketReader
{
public:
  
  ByteBuffer data;
  std::size_t offset;
  
  PacketReader(std::size_t offset = 0) : offset(offset) {}
  PacketReader(const ByteBuffer& data, std::size_t offset = 0)
        : data(data), offset(offset) {}
  PacketReader(ByteBuffer&& data, std::size_t offset = 0)
        : data(std::move(data)), offset(offset) {}
  
  bool readBool();
  std::int8_t readByte();
  std::uint8_t readUByte();
  std::int16_t readShort();
  std::uint16_t readUShort();
  std::int32_t readInt();
  std::int64_t readLong();
  float readFloat();
  double readDouble();
  std::string readString();
  std::int32_t readVarInt();
  std::int64_t readVarLong();
  Vector3i readPosition();
  ByteBuffer readByteArray(std::size_t size);
  
  void consumeUShort();
  void consumeInt();
  void consumeString();
  void consumeVarInt();
  
  static PacketReader getFromCompressedPacket(const ByteBuffer& buffer);
  
  template <typename T>
  static T readVarIntPacket(const std::uint8_t* ptr, std::size_t& bytes)
  {
    PacketReader r(ByteBuffer(ptr, 3));
    T result = static_cast<T>(r.readVarInt());
    bytes = r.offset;
    return result;
  }
  
private:
  
  void need(std::size_t bytes);
  
  template <typename T>
  T readNumber()
  {
    static_assert(std::is_fundamental<T>::value, "Not fundamental ?");
    need(sizeof(T));
    T x;
    std::copy(data.begin() + offset, data.begin() + offset + sizeof(T), reinterpret_cast<std::uint8_t*>(&x));
    offset += sizeof(T);
    return x;
  }
  
  template <typename T>
  T readBNumber()
  {
    return boost::endian::big_to_native(readNumber<T>());
  }
  
  std::uint32_t readVarUInt();
  std::uint64_t readVarULong();
};

  
} // namespace redi

#endif // REDI_PACKETREADER_HPP
