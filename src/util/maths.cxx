#include <cmath>
#include "maths.hpp"

namespace redi
{
namespace util
{

float normalizeAngleDegrees(float degrees)
{
  float result = std::fmod(degrees + 180, 360);
  if (result < 0)
  {
    result += 360;
  }
  return result - 180;
}
  
} // namespace util
} // namespace redi