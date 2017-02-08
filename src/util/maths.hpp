#ifndef REDI_UTILS_MATHS_HPP
#define REDI_UTILS_MATHS_HPP

#include <cmath>
#include <cstdint>

namespace redi
{
namespace util
{

double normalizeAngleDegrees(double degrees);

template <typename To = std::int16_t, typename From>
To floorAndCast(From x)
{
  return static_cast<To>(std::floor(x));
}
  
} // namespace util
} // namespace redi

#endif // REDI_UTILS_MATHS_HPP