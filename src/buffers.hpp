#ifndef REDI_BUFFERS_HPP
#define REDI_BUFFERS_HPP

#include <cstddef>
#include <cstring>
#include "bytebuffer.hpp"
#include "datatypes.hpp"

namespace redi {

class MutableBuffer {
public:
  using value_type = byte*;
  using const_value_type = const byte*;
  using const_iterator = const byte*;
  using iterator = byte*;
  
  explicit MutableBuffer() : ptr(nullptr), ptrsize(0) {}

  explicit MutableBuffer(const MutableBuffer&) = default;
  
  explicit MutableBuffer(value_type data, std::size_t size)
      : ptr(data), ptrsize(size) {}
  
  explicit MutableBuffer(void* data, std::size_t size)
      : MutableBuffer(reinterpret_cast<value_type>(data), size) {}
  
  explicit MutableBuffer(ByteBuffer& buf)
      : MutableBuffer(buf.data(), buf.size()) {}
  
  explicit MutableBuffer(std::string& str)
      : MutableBuffer(&str[0], str.size()) {}
  
  explicit MutableBuffer(char* str)
      : MutableBuffer(str, std::strlen(str)) {}

  MutableBuffer& operator=(const MutableBuffer&) = default;
  
  value_type data() { return ptr; }
  const_value_type data() const { return ptr; }

  char* asChar() { return reinterpret_cast<char*>(data()); }
  const char* asConstChar() const { return reinterpret_cast<const char*>(data()); }
  
  std::size_t size() const { return ptrsize; }
  
  iterator begin() { return data(); }
  iterator end() { return begin() + size(); }
  
  const_iterator begin() const { return data(); }
  const_iterator end() const { return begin() + size(); }

  ByteBuffer toByteBuffer() const { return ByteBuffer(data(), size()); }

private:
  value_type ptr;
  std::size_t ptrsize;
};

class ConstBuffer {
public:
  using value_type = const byte*;
  using const_iterator = const byte*;
  using iterator = byte*;
  
  explicit ConstBuffer() : ptr(nullptr), ptrsize(0) {}
  
  ConstBuffer(value_type data, std::size_t size)
      : ptr(data), ptrsize(size) {}
  
  ConstBuffer(const void* data, std::size_t size)
      : ConstBuffer(reinterpret_cast<value_type>(data), size) {}
  
  explicit ConstBuffer(MutableBuffer& buffer)
      : ConstBuffer(buffer.data(), buffer.size()) {}
  
  ConstBuffer(const ByteBuffer& buf)
      : ConstBuffer(buf.data(), buf.size()) {}
  
  explicit ConstBuffer(const std::string& str)
      : ConstBuffer(&str[0], str.size()) {}
  
  explicit ConstBuffer(const char* str)
      : ConstBuffer(str, std::strlen(str)) {}

  template <typename T, std::size_t Size, typename = std::enable_if_t<sizeof(T) == 1>>
  explicit ConstBuffer(const std::array<T, Size>& array)
      : ConstBuffer(array.data(), array.size()) {}

  const byte& operator[](size_t index) const { return ptr[index]; }

  value_type data() const { return ptr; }
  
  const char* asConstChar() const { return reinterpret_cast<const char*>(ptr); }
  
  std::size_t size() const { return ptrsize; }
  
  const_iterator begin() const { return reinterpret_cast<const byte*>(ptr); }
  const_iterator end() const { return begin() + ptrsize; }

  ByteBuffer toByteBuffer() const { return ByteBuffer(data(), size()); }
private:
  value_type ptr;
  std::size_t ptrsize;
};

}

#endif // REDI_BUFFERS_HPP
