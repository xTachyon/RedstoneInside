#include "varint.hpp"

namespace redi::protocol::varint {

bool is_complete(ConstBuffer buffer) {
  for (byte b : buffer) {
    if ((b & 0b10000000) == 0) {
      return true;
    }
  }
  return false;
}

} // namespace redi