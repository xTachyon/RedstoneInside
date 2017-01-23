#include <cstring>
#include "bytebuffer.hpp"

namespace redi
{

ByteBuffer::ByteBuffer(const char* ptr)
{
  append(ptr, std::strlen(ptr));
  // I didn't want <cstring> in header ..
}

ByteBuffer& operator+=(ByteBuffer& l, const ByteBuffer& r)
{
  l.append(r.data(), r.size());
  
  return l;
}

ByteBuffer operator+(ByteBuffer l, const ByteBuffer& r)
{
  l += r;
  return l;
}

} // namespace redi