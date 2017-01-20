#include <iostream>
#include "../compressor.hpp"
#include "packetwriter.hpp"

namespace redi
{

PacketWriter::PacketWriter(std::int32_t packetid)
{
  writeVarInt(packetid);
}

void PacketWriter::writeBool(bool v)
{
  data.push_back(v ? 1 : 0);
}

void PacketWriter::writeByte(std::int8_t v)
{
  data.push_back(*reinterpret_cast<std::uint8_t*>(std::addressof(v)));
}

void PacketWriter::writeUByte(std::uint8_t v)
{
  data.push_back(v);
}

void PacketWriter::writeShort(std::int16_t v)
{
  writeBNumber(v);
}

void PacketWriter::writeUShort(std::uint16_t v)
{
  writeBNumber(v);
}

void PacketWriter::writeInt(std::int32_t v)
{
  writeBNumber(v);
}

void PacketWriter::writeLong(std::int64_t v)
{
  writeBNumber(v);
}

void PacketWriter::writeULong(std::uint64_t v)
{
  writeBNumber(v);
}

void PacketWriter::writeFloat(float v)
{
  std::int32_t c;
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(v));
  std::copy(ptr, ptr + sizeof(v), reinterpret_cast<std::uint8_t*>(std::addressof(c)));
  writeBNumber(c);
}

void PacketWriter::writeDouble(double v)
{
  std::int64_t c;
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(v));
  std::copy(ptr, ptr + sizeof(v), reinterpret_cast<std::uint8_t*>(std::addressof(c)));
  writeBNumber(c);
}

void PacketWriter::writeString(const std::string& v)
{
  writeVarInt(v.size());
  data.append(reinterpret_cast<const std::uint8_t*>(v.data()), v.size());
}

void PacketWriter::writeVarInt(std::int32_t v)
{
  writeVarInt(*reinterpret_cast<std::uint32_t*>(std::addressof(v)));
}

void PacketWriter::writeVarInt(std::uint32_t v)
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


void PacketWriter::writeVarLong(std::int64_t v)
{
  writeVarULong(*reinterpret_cast<std::uint64_t*>(std::addressof(v)));
}

void PacketWriter::writeVarULong(std::uint64_t v)
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


void PacketWriter::writePosition(std::int64_t x, std::int64_t y, std::int64_t z)
{
  writeLong(((x & 0x3FFFFFF) << 38) | ((y & 0xFFF) << 26) | (z & 0x3FFFFFF));
}

void PacketWriter::commit(bool)
{
//  std::cout << (int)data[0] << " -- " << std::boolalpha << compressed << '\n';
//  if (compressed)
//  {
////    ByteBuffer comp = compressor::compressZlib(data);
////    PacketWriter wr;
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