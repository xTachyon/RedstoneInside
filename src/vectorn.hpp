#ifndef REDI_POSITION
#define REDI_POSITION

#include <cstdint>

namespace redi
{

struct Position
{
  double x;
  double y;
  double z;
  
  Position(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x), y(y), z(z) {}
};

struct Location : public Position
{
  using Position::Position;
};

template <typename T>
struct Vector2
{
  T x, z;
  
  Vector2(T x = 0, T z = 0)
        : x(x), z(z) {}
  
  bool operator==(const Vector2& r) const
  {
    return x == r.x && z = r.z;
  }
  
  bool operator<(const Vector2& r) const
  {
    return x < r.x || (x == r.x && z < r.z);
  }
};

template <typename T>
struct Vector3
{
  T x, y, z;
  
  Vector3(T x = 0, T y = 0, T z = 0)
        : x(x), y(y), z(z) {}
  
  bool operator==(const Vector3& r) const
  {
    return x == r.x && y == r.y && z == r.z;
  }
};

using Vector2i = Vector2<std::int32_t>;
using Vector3i = Vector3<std::int32_t>;
using Vector3d = Vector3<double>;
  
} // namespace redi

#endif // REDI_POSITION