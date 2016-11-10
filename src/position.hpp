#ifndef REDI_POSITION
#	define REDI_POSITION

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
  Location(double x = 0.0, double y = 0.0, double z = 0.0)
    : Position(x, y, z) {}
};

} // namespace redi

#endif // REDI_POSITION