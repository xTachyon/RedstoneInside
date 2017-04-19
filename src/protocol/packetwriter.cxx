#include "../compressor.hpp"
#include "../chat/chatcomponent.hpp"
#include "packetwriter.hpp"

namespace redi {

PacketWriter::PacketWriter(ByteBuffer& data, std::int32_t packetid)
    : data(data) {
  writeVarInt(packetid);
}

PacketWriter::~PacketWriter() { commit(); }

void PacketWriter::writeBool(bool v) { data.push_back(v ? 1 : 0); }

void PacketWriter::writeByte(std::int8_t v) {
  data.push_back(*reinterpret_cast<std::uint8_t*>(std::addressof(v)));
}

void PacketWriter::writeUByte(std::uint8_t v) { data.push_back(v); }

void PacketWriter::writeShort(std::int16_t v) { writeBNumber(v); }

void PacketWriter::writeUShort(std::uint16_t v) { writeBNumber(v); }

void PacketWriter::writeInt(std::int32_t v) { writeBNumber(v); }

void PacketWriter::writeLong(std::int64_t v) { writeBNumber(v); }

void PacketWriter::writeULong(std::uint64_t v) { writeBNumber(v); }

void PacketWriter::writeFloat(float v) {
  std::int32_t c;
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(v));
  std::copy(ptr, ptr + sizeof(v),
            reinterpret_cast<std::uint8_t*>(std::addressof(c)));
  writeBNumber(c);
}

void PacketWriter::writeDouble(double v) {
  std::int64_t c;
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(v));
  std::copy(ptr, ptr + sizeof(v),
            reinterpret_cast<std::uint8_t*>(std::addressof(c)));
  writeBNumber(c);
}

void PacketWriter::writeString(string_view str) {
  writeVarInt(str.size());
  data.append(reinterpret_cast<const std::uint8_t*>(str.data()), str.size());
}

void PacketWriter::writeVarInt(std::int32_t v) {
  writeVarInt(*reinterpret_cast<std::uint32_t*>(std::addressof(v)));
}

void PacketWriter::writeVarInt(std::uint32_t v) {
  do {
    std::uint8_t temp = std::uint8_t(v & 0b01111111);
    v >>= 7;
    temp |= v ? 0b10000000 : 0;
    writeUByte(temp);
  } while (v);
}

void PacketWriter::writeVarLong(std::int64_t v) {
  writeVarULong(*reinterpret_cast<std::uint64_t*>(std::addressof(v)));
}

void PacketWriter::writeVarULong(std::uint64_t v) {
  do {
    std::uint8_t temp = std::uint8_t(v & 0b01111111);
    v >>= 7;
    temp |= v ? 0b10000000 : 0;
    writeUByte(temp);
  } while (v);
}

void PacketWriter::writePosition(std::int64_t x, std::int64_t y,
                                 std::int64_t z) {
  writeLong(((x & 0x3FFFFFF) << 38) | ((y & 0xFFF) << 26) | (z & 0x3FFFFFF));
}

void PacketWriter::writeUUID(boost::uuids::uuid uuid) {
  data.append(uuid.data, sizeof(uuid.data));
}

void PacketWriter::writeAngle(double angle) {
  writeByte(static_cast<std::int8_t>(255 * angle / 360));
}

void PacketWriter::commit(bool) {
  ByteBuffer d(std::move(data));

  writeVarInt(d.size());
  data.append(d.data(), d.size());
}

void PacketWriter::writeChat(const chat::ChatComponent& chat) {
  writeString(chat.generate());
}

//void PacketWriter::writeChat(std::string&& str) {
//  writeChat(chat::ChatComponent(std::move(str)));
//}
//
//void PacketWriter::writeChat(string_view str) {
//  writeChat(chat::ChatComponent(str));
//}

} // namespace redi