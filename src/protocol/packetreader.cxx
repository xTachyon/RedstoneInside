#include <stdexcept>
#include <boost/endian/conversion.hpp>
#include "packetreader.hpp"

namespace endian = boost::endian;

namespace redi
{

bool PacketReader::readBool()
{
  need(sizeof(bool));
  return data[offset++] == 1;
}

std::int8_t PacketReader::readByte()
{
  need(sizeof(std::int8_t));
  return *reinterpret_cast<std::int8_t*>(std::addressof(data[offset++]));
}

std::uint8_t PacketReader::readUByte()
{
  need(sizeof(std::uint8_t));
  return data[offset++];
}

std::int16_t PacketReader::readShort()
{
  return readBNumber<std::int16_t>();
}

std::uint16_t PacketReader::readUShort()
{
  return readBNumber<std::uint16_t>();
}

std::int32_t PacketReader::readInt()
{
  return readBNumber<std::uint32_t>();
}

std::int64_t PacketReader::readLong()
{
  return readBNumber<std::uint64_t>();
}

float PacketReader::readFloat()
{
  std::int32_t i = readBNumber<std::uint32_t>();
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(i));
  float x;
  std::copy(ptr, ptr + sizeof(float), reinterpret_cast<std::uint8_t*>(std::addressof(x)));
  return x;
}

double PacketReader::readDouble()
{
  std::int64_t i = readBNumber<std::uint64_t>();
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(i));
  double x;
  std::copy(ptr, ptr + sizeof(double), reinterpret_cast<std::uint8_t*>(std::addressof(x)));
  return x;
}

std::string PacketReader::readString()
{
  std::size_t size = static_cast<std::size_t>(readVarInt());
  need(size);
  std::string result(reinterpret_cast<const char*>(std::addressof(data[offset])), size);
  offset += size;
  return result;
}

std::uint32_t PacketReader::readVarUInt()
{
  std::uint32_t Value = 0;
  int Shift = 0;
  unsigned char b = 0;
  do
  {
    b = readUByte();
    Value = Value | ((static_cast<std::uint32_t>(b & 0x7f)) << Shift);
    Shift += 7;
  } while ((b & 0x80) != 0);
  return Value;
}

std::int32_t PacketReader::readVarInt()
{
  std::uint32_t x = readVarUInt();
  return *reinterpret_cast<std::int32_t*>(std::addressof(x));
}

std::uint64_t PacketReader::readVarULong()
{
  std::uint64_t Value = 0;
  int Shift = 0;
  unsigned char b = 0;
  do
  {
    b = readUByte();
    Value = Value | ((static_cast<std::uint64_t>(b & 0x7f)) << Shift);
    Shift += 7;
  } while ((b & 0x80) != 0);
  return Value;
  
}

std::int64_t PacketReader::readVarLong()
{
  std::uint64_t x = readVarULong();
  return *reinterpret_cast<std::int64_t*>(std::addressof(x));
}

void PacketReader::need(std::size_t bytes)
{
  if (bytes + offset > data.size())
    throw std::runtime_error(std::to_string(bytes) + " is out of index");
}

void PacketReader::consumeUShort()
{
  need(sizeof(std::uint16_t));
  offset += sizeof(std::uint16_t);
}

void PacketReader::consumeInt()
{
  need(sizeof(std::int32_t));
  offset += sizeof(std::int32_t);
}

void PacketReader::consumeString()
{
  std::size_t size = static_cast<std::size_t>(readVarInt());
  need(size);
  offset += size;
}

void PacketReader::consumeVarInt()
{
  static_cast<void>(readVarInt());
}
  
} // namespace redi