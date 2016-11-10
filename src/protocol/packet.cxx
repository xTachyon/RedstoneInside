#include <boost/endian/conversion.hpp>
#include "packet.hpp"
#include "../logger.hpp"

namespace endian = boost::endian;

namespace redi
{
namespace protocol
{
Packet::Packet(std::int32_t packetID)
  : readIndex(0), startsAt(0)
{
  Logger::debugSync(data.size());
  writeVarInt(1 << 30);
  Logger::debugSync(data.size());
  writeVarInt(packetID);
  Logger::debugSync(data.size());
}

Packet::Packet(HandshakePacketType t)
  : Packet(static_cast<std::int32_t>(t)) {}

Packet::Packet(PlayPacketType t)
  : Packet(static_cast<std::int32_t>(t)) {}

Packet::Packet(StatusPacketType t)
  : Packet(static_cast<std::int32_t>(t)) {}

void Packet::writeBool(bool data)
{
  this->data.push_back(data ? 1 : 0);
}

void Packet::writeSignedByte(std::int8_t data)
{
  writeRaw(data);
}

void Packet::writeUnsignedByte(std::uint8_t data)
{
  this->data.push_back(data);
}

void Packet::writeBigShort(std::int16_t data)
{
  writeRaw(endian::native_to_big(data));
}

void Packet::writeBigUnsignedShort(std::uint16_t data)
{
  writeRaw(endian::native_to_big(data));
}

void Packet::writeBigInt(std::int32_t data)
{
  writeRaw(endian::native_to_big(data));
}

void Packet::writeBigLong(std::uint64_t data)
{
  writeRaw(endian::native_to_big(data));
}

void Packet::writeBigFloat(float data)
{
  writeRaw(*reinterpret_cast<std::int32_t*>(&data));
}

void Packet::writeBigDouble(double data)
{
  writeRaw(*reinterpret_cast<std::int64_t*>(&data));
}

void Packet::writeVarInt(std::int32_t data)
{
  BinaryData res(writeVarIntInternal(data));
  this->data.append(res.data(), res.size());
}

void Packet::writeVarLong(std::int64_t data)
{
  BinaryData res(writeVarIntInternal(data));
  this->data.append(res.data(), res.size());
}

void Packet::writeString(const std::string& data)
{
  writeVarInt(static_cast<std::int32_t>(data.size()));
  this->data.append(reinterpret_cast<const std::uint8_t*>(data.data()), data.size());
}

bool Packet::readBool()
{
  assert(data[readIndex] == 0 || data[readIndex] == 1);
  need(sizeof(bool));
  return data[readIndex++] == 1;
}

std::int8_t Packet::readSygnedByte()
{
  need(sizeof(std::int8_t));
  return *reinterpret_cast<std::int8_t*>(&data[readIndex++]);
}

std::uint8_t Packet::readUnsignedByte()
{
  need(sizeof(std::uint8_t));
  return data[readIndex++];
}

std::int16_t Packet::readBigShort()
{
  need(sizeof(std::int16_t));
  std::int16_t result = endian::big_to_native(*reinterpret_cast<const std::int16_t*>(&data[readIndex]));
  readIndex += sizeof(std::int16_t);
  return result;
}

std::uint16_t Packet::readBigUnsignedShort()
{
  need(sizeof(std::uint16_t));
  std::uint16_t result = endian::big_to_native(*reinterpret_cast<const std::uint16_t*>(&data[readIndex]));
  readIndex += sizeof(std::uint16_t);
  return result;
}

std::int32_t Packet::readBigInt()
{
  need(sizeof(std::int32_t));
  std::int32_t result = endian::big_to_native(*reinterpret_cast<const std::int32_t*>(&data[readIndex]));
  readIndex += sizeof(std::int32_t);
  return result;
}

std::int64_t Packet::readBigLong()
{
  need(sizeof(std::int64_t));
  std::int64_t result = endian::big_to_native(*reinterpret_cast<const std::int64_t*>(&data[readIndex]));
  readIndex += sizeof(std::int64_t);
  return result;
}

float Packet::readBigFloat()
{
  need(sizeof(float));
  std::int32_t result = endian::big_to_native(readBigInt());
  return *reinterpret_cast<float*>(&result);
}

double Packet::readBigDouble()
{
  need(sizeof(double));
  std::int64_t result = endian::big_to_native(readBigLong());
  return *reinterpret_cast<double*>(&result);
}

std::int32_t Packet::readVarInt()
{
  return static_cast<std::int32_t>(readVarIntInternal());
}

std::int64_t Packet::readVarLong()
{
  return readVarIntInternal();
}

std::string Packet::readString()
{
  std::size_t size = readVarInt();
  need(size);
  readIndex += size;
  return std::string(reinterpret_cast<const char*>(&data[readIndex - size]), size);
}

void Packet::finish()
{
  BinaryData res(writeVarIntInternal(data.size() - 5));
  for (std::size_t i = 0, j = 5 - res.size(); i < res.size(); ++i, ++j)
    data[j] = res[i];
  startsAt = readIndex = 5 - res.size();
}

BinaryData Packet::writeVarIntInternal(std::int64_t data)
{
  std::uint8_t buffer[10];
  std::size_t i = 0;

  while ((data & -128) != 0)
  {
    buffer[i++] = (data & 127) | 128;
    data >>= 7;
  }
  buffer[i++] = static_cast<std::uint8_t>(data);

  return BinaryData(buffer, i);
}

std::int64_t Packet::readVarIntInternal()
{
  std::int64_t output = 0;
  int v2 = 0;
  std::int8_t v3;

  do
  {
    if (v2 + readIndex >= data.size()) break;
    v3 = data[readIndex++];
    output |= (v3 & 127) << (v2++ * 7);
    if (v2 > 10) return v2;
  } while ((v3 & 128) == 128);

  //readIndex += v2;
  return output;
}

void Packet::need(std::size_t bytes)
{
  if (bytes + readIndex > data.size())
    throw std::invalid_argument(std::to_string(bytes) + " is out of index");
}

} // namespace protocol
} // namespace redi