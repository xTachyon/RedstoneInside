#pragma once

#include <cstdint>

namespace redi {
namespace util {

std::int32_t getUnixTimestamp();
std::int64_t getUnixTimestampMilliseconds();

std::int32_t getRandomInt32();

} // namespace util
} // namespace redi