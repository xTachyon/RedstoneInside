#ifndef REDI_BUFFERS_HPP
#define REDI_BUFFERS_HPP

#include "bytebuffer.hpp"
#include "datatypes.hpp"

namespace redi {

class MutableBuffer {
public:
  using const_iterator = const byte*;
  using iterator = byte*;
  
  explicit MutableBuffer() : data(nullptr), size(0) {}
  
  explicit MutableBuffer(void* data, std::size_t size)
      : data(data), size(size) {}
  
  explicit MutableBuffer(ByteBuffer& buf)
      : MutableBuffer(buf.data(), buf.size()) {}
  
  explicit MutableBuffer(std::string& str)
      : MutableBuffer(&str[0], str.size()) {}
  
  explicit MutableBuffer(char* str)
      : MutableBuffer(str, std::strlen(str)) {}
  
  void* getBuffer() { return data; }
  
  std::size_t getSize() const { return size; }
  
  iterator begin() { return reinterpret_cast<byte*>(data); }
  
  iterator end() { return begin() + size; }
  
  const_iterator begin() const { return reinterpret_cast<byte*>(data); }
  
  const_iterator end() const { return begin() + size; }

private:
  void* data;
  std::size_t size;
};

class ConstBuffer {
public:
  using const_iterator = const byte*;
  using iterator = byte*;
  
  explicit ConstBuffer() : data(nullptr), size(0) {}
  
  explicit ConstBuffer(const void* data, std::size_t size)
      : data(data), size(size) {}
  
  explicit ConstBuffer(const ByteBuffer& buf)
      : ConstBuffer(buf.data(), buf.size()) {}
  
  explicit ConstBuffer(const std::string& str)
      : ConstBuffer(&str[0], str.size()) {}
  
  explicit ConstBuffer(const char* str)
      : ConstBuffer(str, std::strlen(str)) {}
  
  const void* getBuffer() { return data; }
  
  std::size_t getSize() const { return size; }
  
  const_iterator begin() const { return reinterpret_cast<const byte*>(data); }
  
  const_iterator end() const { return begin() + size; }

private:
  const void* data;
  std::size_t size;
};


}

#endif // REDI_BUFFERS_HPP
