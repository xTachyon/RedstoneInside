#pragma once

#include <cmath>
#include <cstdint>
#include <boost/format.hpp>

namespace redi {

struct Position {
  double x;
  double y;
  double z;

  Position(double x = 0.0, double y = 0.0, double z = 0.0) : x(x), y(y), z(z) {}
};

struct Location : public Position {
  using Position::Position;
};

template <typename T>
struct Vector2 {
  T x, z;

  Vector2(T x = 0, T z = 0) : x(x), z(z) {}

    bool operator==(const Vector2& r) const {
      return x == r.x && z == r.z;
    }

  bool operator<(const Vector2& r) const {
    return x < r.x || (x == r.x && z < r.z);
  }

  std::string toString() const {
    return std::to_string(x) + ", " + std::to_string(z);
  }

  std::int64_t distanceSquared(const Vector2& r) {
    std::int64_t xx = static_cast<std::int64_t>(x) - r.x;
    std::int64_t zz = static_cast<std::int64_t>(z) - r.z;

    return xx * xx + zz * zz;
  }

  double distance(const Vector2& r) { return std::sqrt(distanceSquared(r)); }
};

template <typename T>
bool operator==(const Vector2<T>& l, const Vector2<T>& r) {
  return l.x == r.x && l.z == r.z;
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Vector2<T>& val) {
  stream << val.toString();
  return stream;
}

template <typename T>
struct Vector3 {
  T x, y, z;

  Vector3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}

  bool operator==(const Vector3& r) const {
    return x == r.x && y == r.y && z == r.z;
  }

  std::string toString() const {
    //    return (boost::format("(%1%, %2%, %3%)") % x % y % z).str();
    return std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z);
  }

  template <typename Result = T>
  Result distanceSquared(const Vector3& r) {
    Result xx = static_cast<Result>(x - r.x);
    Result yy = static_cast<Result>(y - r.y);
    Result zz = static_cast<Result>(z - r.z);

    return xx * xx + yy * yy + zz * zz;
  }

  double distance(const Vector3& r) { return std::sqrt(distanceSquared(r)); }
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Vector3<T>& val) {
  stream << val.toString();
  return stream;
}

using Vector2i = Vector2<std::int32_t>;
using Vector3i = Vector3<std::int32_t>;
using Vector3d = Vector3<double>;

} // namespace redi