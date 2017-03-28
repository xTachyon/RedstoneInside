#ifndef REDI_UTIL_RANDOM_HPP
#define REDI_UTIL_RANDOM_HPP

#include <random>

namespace redi {
namespace util {

template <typename T>
T generateRandom(const T& min, const T& max) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<T> dist(min, max);

  return dist(mt);
}

} // namespace util
} // namespace redi

#endif // REDI_UTIL_RANDOM_HPP