#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace redi {

class ByteBuffer : public std::vector<std::uint8_t> {
public:
  using base = std::vector<std::uint8_t>;

  using base::base;
  using base::operator=;
  using base::operator[];

  ByteBuffer() = default;

  ByteBuffer(const std::uint8_t* ptr, base::size_type size) {
    append(ptr, size);
  }

  ByteBuffer(const char* ptr);

  ByteBuffer(const char* ptr, base::size_type size) { append(ptr, size); }

  template <typename T, std::size_t size = sizeof(T)>
  void append(const T& obj) {
    append(reinterpret_cast<const std::uint8_t*>(std::addressof(obj)), size);
  }

  void append(const std::uint8_t* ptr, base::size_type size) {
    insert(end(), ptr, ptr + size);
  }

  void append(const char* ptr, base::size_type size) {
    append(reinterpret_cast<const std::uint8_t*>(ptr), size);
  }

  char* as_char() { return reinterpret_cast<char*>(data()); }

  const char* as_const_char() const {
    return reinterpret_cast<const char*>(data());
  }
};

ByteBuffer& operator+=(ByteBuffer& l, const ByteBuffer& r);
ByteBuffer operator+(ByteBuffer l, const ByteBuffer& r);

using ByteBufferSharedPtr = std::shared_ptr<ByteBuffer>;

} // namespace redi