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
//  need(sizeof(std::int16_t));
//  std::int16_t x;
//  std::copy(std::addressof(data[offset]), std::addressof(data[offset + sizeof(std::int16_t)]), reinterpret_cast<std::uint8_t*>(std::addressof(x)));
//  return endian::big_to_native(x);
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

std::int32_t PacketReader::readVarInt()
{
  std::int32_t numRead = 0;
  std::int32_t result = 0;
  std::int8_t read;
  do
  {
    read = readByte();
    std::int32_t value = (read & 0b01111111);
    result |= (value << (7 * numRead));
    
    numRead++;
    if (numRead > 5)
    {
      throw new std::runtime_error("VarInt is too big");
    }
  } while ((read & 0b10000000) != 0);
  
  return result;
}

std::int64_t PacketReader::readVarLong()
{
  std::int32_t numRead = 0;
  std::int32_t result = 0;
  std::int8_t read;
  do
  {
    read = readByte();
    std::int32_t value = (read & 0b01111111);
    result |= (value << (7 * numRead));
    
    numRead++;
    if (numRead > 10)
    {
      throw new std::runtime_error("VarLong is too big");
    }
  } while ((read & 0b10000000) != 0);
  
  return result;
}

void PacketReader::need(std::size_t bytes)
{
  if (bytes + offset > data.size())
    throw std::runtime_error(std::to_string(bytes) + " is out of index");
}

} // namespace redi