#ifndef REDI_UTIL_TIME
#define REDI_UTIL_TIME

#include <cstdint>

namespace redi
{
namespace util
{

std::int32_t getUnixTimestamp();
std::int64_t getUnixTimestampMilliseconds();
  
} // namespace util
} // namespace redi

#endif // REDI_UTIL_TIME
