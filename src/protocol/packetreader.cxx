#include <stdexcept>
#include <boost/endian/conversion.hpp>
#include "packetreader.hpp"
#include "../compressor.hpp"

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
  std::uint32_t result = 0;
  int shift = 0;
  std::uint8_t b = 0;
  
  do
  {
    b = readUByte();
    result = result | ((static_cast<std::uint32_t>(b & 0x7f)) << shift);
    shift += 7;
  } while ((b & 0x80) != 0);
  
  return result;
}

std::int32_t PacketReader::readVarInt()
{
  std::uint32_t x = readVarUInt();
  return *reinterpret_cast<std::int32_t*>(std::addressof(x));
}

std::uint64_t PacketReader::readVarULong()
{
  std::uint64_t result = 0;
  int shift = 0;
  std::uint8_t b = 0;
  
  do
  {
    b = readUByte();
    result = result | ((static_cast<std::uint64_t>(b & 0x7f)) << shift);
    shift += 7;
  } while ((b & 0x80) != 0);
  
  return result;
}

std::int64_t PacketReader::readVarLong()
{
  std::uint64_t x = readVarULong();
  return *reinterpret_cast<std::int64_t*>(std::addressof(x));
}

Vector3i PacketReader::readPosition()
{
  Vector3i result;
  
  std::int64_t raw = readLong();
  std::uint32_t rx = static_cast<std::uint32_t>((raw >> 38) & 0x03ffffff);
  std::uint32_t ry = static_cast<std::uint32_t>((raw >> 26) & 0x0fff);
  std::uint32_t rz = static_cast<std::uint32_t>(raw & 0x03ffffff);
  
  result.x = (rx & 0x02000000) == 0 ? static_cast<std::int32_t>(rx) : -(0x04000000 - static_cast<std::int32_t>(rx));
  result.y = (ry & 0x0800)     == 0 ? static_cast<std::int32_t>(ry) : -(0x0800     - static_cast<std::int32_t>(ry));
  result.z = (rz & 0x02000000) == 0 ? static_cast<std::int32_t>(rz) : -(0x04000000 - static_cast<std::int32_t>(rz));
  
  return result;
}

ByteBuffer PacketReader::readByteArray(std::size_t size)
{
  need(size);
  ByteBuffer result(data.data(), size);
  offset += size;
  return result;
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

PacketReader PacketReader::getFromCompressedPacket(const ByteBuffer& buf)
{
  PacketReader reader(buf);
  
  std::size_t len = static_cast<std::size_t>(reader.readVarInt());
  ByteBuffer buffer = compressor::decompressZlib(ByteBuffer(reader.data.begin() + reader.offset, reader.data.end()));
  if (len != buffer.size()) throw std::runtime_error("Uncompressed length is not equal with the one specified");
  
  return PacketReader(std::move(buffer));
}
  
} // namespace redi