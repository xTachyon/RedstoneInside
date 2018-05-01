#pragma once

#include <cstddef>
#include "../datatypes.hpp"
#include "../buffers.hpp"

namespace redi::protocol::varint {

template <typename T>
std::size_t encodeVarInt(MutableBuffer buffer, T value) {
  constexpr byte mask = 0b01111111;

  std::size_t size = 0;
  do {
    byte temp = value & mask;
    value >>= 7;
    if (value != 0) {
      temp |= 0b10000000;
    }
    buffer.data()[size++] = temp;
  } while (value > 0);
  return size;
}

template <typename T>
std::pair<std::array<byte, 10>, std::size_t> encodeVarInt(T x) {
  std::pair<std::array<byte, 10>, std::size_t> result;

  result.second = encodeVarInt(MutableBuffer(result.first.data(), result.first.size()), x);

  return result;
}

} // namespace redi