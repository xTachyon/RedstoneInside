#include "../compressor.hpp"
#include "packetwriternocopy.hpp"

namespace redi
{

PacketWriterNoCopy::PacketWriterNoCopy(ByteBuffer& data, std::int32_t packetid) : data(data)
{
  writeVarInt(packetid);
}

PacketWriterNoCopy::~PacketWriterNoCopy()
{
  commit();
}

void PacketWriterNoCopy::writeBool(bool v)
{
  data.push_back(v ? 1 : 0);
}

void PacketWriterNoCopy::writeByte(std::int8_t v)
{
  data.push_back(*reinterpret_cast<std::uint8_t*>(std::addressof(v)));
}

void PacketWriterNoCopy::writeUByte(std::uint8_t v)
{
  data.push_back(v);
}

void PacketWriterNoCopy::writeShort(std::int16_t v)
{
  writeBNumber(v);
}

void PacketWriterNoCopy::writeUShort(std::uint16_t v)
{
  writeBNumber(v);
}

void PacketWriterNoCopy::writeInt(std::int32_t v)
{
  writeBNumber(v);
}

void PacketWriterNoCopy::writeLong(std::int64_t v)
{
  writeBNumber(v);
}

void PacketWriterNoCopy::writeULong(std::uint64_t v)
{
  writeBNumber(v);
}

void PacketWriterNoCopy::writeFloat(float v)
{
  std::int32_t c;
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(v));
  std::copy(ptr, ptr + sizeof(v), reinterpret_cast<std::uint8_t*>(std::addressof(c)));
  writeBNumber(c);
}

void PacketWriterNoCopy::writeDouble(double v)
{
  std::int64_t c;
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(v));
  std::copy(ptr, ptr + sizeof(v), reinterpret_cast<std::uint8_t*>(std::addressof(c)));
  writeBNumber(c);
}

void PacketWriterNoCopy::writeString(const std::string& v)
{
  writeVarInt(v.size());
  data.append(reinterpret_cast<const std::uint8_t*>(v.data()), v.size());
}

void PacketWriterNoCopy::writeVarInt(std::int32_t v)
{
  writeVarInt(*reinterpret_cast<std::uint32_t*>(std::addressof(v)));
}

void PacketWriterNoCopy::writeVarInt(std::uint32_t v)
{
  do
  {
    std::int8_t temp = static_cast<std::uint8_t>(v & 0b01111111);
    v >>= 7;
    if (v != 0)
    {
      temp |= 0b10000000;
    }
    writeByte(temp);
  } while (v != 0);
}


void PacketWriterNoCopy::writeVarLong(std::int64_t v)
{
  writeVarULong(*reinterpret_cast<std::uint64_t*>(std::addressof(v)));
}

void PacketWriterNoCopy::writeVarULong(std::uint64_t v)
{
  do
  {
    std::uint8_t temp = static_cast<std::uint8_t>(v & 0b01111111);
    v >>= 7;
    if (v != 0)
    {
      temp |= 0b10000000;
    }
    writeByte(temp);
  } while (v != 0);
}


void PacketWriterNoCopy::writePosition(std::int64_t x, std::int64_t y, std::int64_t z)
{
  writeLong(((x & 0x3FFFFFF) << 38) | ((y & 0xFFF) << 26) | (z & 0x3FFFFFF));
}

void PacketWriterNoCopy::writeUUID(boost::uuids::uuid uuid)
{
  data.append(uuid.data, sizeof(uuid.data));
}

void PacketWriterNoCopy::writeAngle(double angle)
{
  writeByte(static_cast<std::int8_t>(255 * angle / 360));
}

void PacketWriterNoCopy::commit(bool)
{
//  std::cout << (int)data[0] << " -- " << std::boolalpha << compressed << '\n';
//  if (compressed)
//  {
////    ByteBuffer comp = compressor::compressZlib(data);
////    PacketWriterNoCopy wr;
////    wr.writeVarInt(comp.size());
////
////    data.clear();
////    writeVarInt(wr.data.size() + comp.size());
////    data += wr.data;
////    data += comp;
//    ByteBuffer original = data;
//    data.clear();
//    writeVarInt(original.size() + 1);
//    writeVarInt(0);
//    data += original;
//  }
//  else
//  {
    ByteBuffer d(std::move(data));
  
    writeVarInt(d.size());
    data.append(d.data(), d.size());
//  }
}

} // namespace redi