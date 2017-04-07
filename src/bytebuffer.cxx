#include <cstring>
#include "bytebuffer.hpp"

namespace redi {

ByteBuffer::ByteBuffer(const char* ptr) {
  // I didn't want <cstring> in header ..
  append(ptr, std::strlen(ptr));
}

ByteBuffer& operator+=(ByteBuffer& l, const ByteBuffer& r) {
  l.append(r.data(), r.size());

  return l;
}

ByteBuffer operator+(ByteBuffer l, const ByteBuffer& r) {
  l += r;
  return l;
}

} // namespace redi