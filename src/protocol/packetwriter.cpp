#include "../chat/chatcomponent.hpp"
#include "packetwriter.hpp"

namespace redi {

PacketWriter::PacketWriter(ByteBuffer& data, std::int32_t packetid)
    : data(data) {
  writeVarInt(packetid);
}

void PacketWriter::writeBool(bool v) {
  constexpr u8 true_value = 1;
  constexpr u8 false_value = 0;
  data.push_back(v ? true_value : false_value);
}

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

void PacketWriter::writeString(const std::string& str) {
  writeVarInt(str.size());
  data.append(reinterpret_cast<const std::uint8_t*>(str.data()), str.size());
}

void PacketWriter::writeVarInt(std::int32_t v) {
  writeVarInt(*reinterpret_cast<std::uint32_t*>(std::addressof(v)));
}

void PacketWriter::writeVarInt(std::uint32_t v) {
  do {
    auto temp = std::uint8_t(v & 0b01111111);
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
    auto temp = std::uint8_t(v & 0b01111111);
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

void PacketWriter::writeChat(const chat::ChatComponent& chat) {
  writeString(chat.generate());
}

template <typename T>
void PacketWriter::writeNumber(T number) {
  data.append(reinterpret_cast<const std::uint8_t*>(std::addressof(number)),
              sizeof(T));
}

template <typename T>
void PacketWriter::writeBNumber(T number) {
  writeNumber(boost::endian::native_to_big(number));
}

} // namespace redi