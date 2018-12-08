#pragma once

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

template <typename T = std::mt19937>
class BasicRandom {
public:
  BasicRandom()
      : BasicRandom(std::random_device()()) {}
  
  BasicRandom(typename T::result_type seed) {
    rr.seed(seed);
  }
  
  template <typename K = typename T::result_type>
  K nextInteger(K min = 0, K max = std::numeric_limits<K>::max()) {
    std::uniform_int_distribution<K> dist(min, max);
    return dist(rr);
  }
  
  template <typename K = double>
  K nextReal() {
    std::uniform_real_distribution<K> dist;
    return dist(rr);
  }

private:
  T rr;
};

using Random = BasicRandom<std::mt19937>;

} // namespace util
} // namespace redi