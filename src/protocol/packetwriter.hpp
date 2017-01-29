#ifndef REDI_PACKETWRITER_HPP
#define REDI_PACKETWRITER_HPP

#include <string>
#include <boost/endian/conversion.hpp>
#include <boost/uuid/uuid.hpp>
#include "../bytebuffer.hpp"

namespace redi
{

class PacketWriter
{
public:
  
  ByteBuffer data;
  
  PacketWriter(std::int32_t packetid);
  
  operator ByteBuffer&&() { return std::move(data); }
  
  void writeBool(bool v);
  void writeByte(std::int8_t v);
  void writeUByte(std::uint8_t v);
  void writeShort(std::int16_t v);
  void writeUShort(std::uint16_t v);
  void writeInt(std::int32_t v);
  void writeLong(std::int64_t v);
  void writeULong(std::uint64_t v);
  void writeFloat(float v);
  void writeDouble(double v);
  void writeString(const std::string& v);
  void writeVarInt(std::int32_t v);
  void writeVarInt(std::uint32_t v);
  void writeVarLong(std::int64_t v);
  void writeVarULong(std::uint64_t v);
  void writePosition(std::int64_t x, std::int64_t y, std::int64_t z);
  void writeUUID(boost::uuids::uuid uuid);
  void commit(bool compressed = false);
  
  template <typename T>
  void writeVarInt(T number)
  {
    writeVarInt(static_cast<std::int32_t>(number));
  }
  
  template <typename T>
  void writeVarLong(T number)
  {
    writeVarLong(static_cast<std::int64_t>(number));
  }
  
  template <typename T, typename K, typename L>
  void writePosition(T x, K y, L z)
  {
    writePosition(static_cast<std::int64_t>(x), static_cast<std::int64_t>(y), static_cast<std::int64_t>(z));
  }
  
private:
  
  PacketWriter() = default;
  
  template <typename T>
  void writeNumber(T number)
  {
    data.append(reinterpret_cast<const std::uint8_t*>(std::addressof(number)), sizeof(T));
  }
  
  template <typename T>
  void writeBNumber(T number)
  {
    writeNumber(boost::endian::native_to_big(number));
  }
};
  
} // namespace redi

#endif // REDI_PACKETWRITER_HPP
