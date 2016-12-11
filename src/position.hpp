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

struct BlockPosition
{
  std::int32_t x;
  std::int32_t y;
  std::int32_t z;
  
  BlockPosition(std::int32_t x = 0, std::int32_t y = 0, std::int32_t z = 0)
        : x(x), y(y), z(z) {}
};

struct Location : public Position
{
  using Position::Position;
};

struct Vector2
{
  std::int32_t x, z;

  Vector2(std::int32_t x = 0, std::int32_t z = 0)
    : x(x), z(z) {}
};

} // namespace redi

#endif // REDI_POSITION