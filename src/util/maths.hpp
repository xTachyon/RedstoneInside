#pragma once

#include <cmath>
#include <cstdint>
#include <utility>

namespace redi {
namespace util {

float normalizeAngleDegrees(float degrees);

template <typename To = std::int16_t, typename From>
To floorAndCast(From x) {
  return static_cast<To>(std::floor(x));
}

template <typename T, typename K>
inline std::uint8_t createNibble(const T& l, const K& r) {
  std::uint8_t x = static_cast<std::uint8_t>(l);
  std::uint8_t y = static_cast<std::uint8_t>(r);

  return (x << 4) | y;
}

template <typename T>
inline std::pair<std::uint8_t, std::uint8_t> splitNibble(const T& l) {
  std::uint8_t x = static_cast<std::uint8_t>(l);

  std::pair<std::uint8_t, std::uint8_t> result;
  result.first = x >> 4;
  result.second = static_cast<std::uint8_t>(x & 0b1111);

  return result;
}

} // namespace util
} // namespace redi