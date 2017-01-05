#ifndef REDI_BASE64_HPP
#define REDI_BASE64_HPP

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include "../bytebuffer.hpp"

namespace redi
{
namespace util
{
namespace base64
{

ByteBuffer encode(const ByteBuffer& data);
ByteBuffer encode(const std::string& str);
std::string encodeToString(const std::string& str);

ByteBuffer decode(const ByteBuffer& data);
ByteBuffer decode(const std::string& str);
std::string decodeToString(const ByteBuffer& data);
std::string decodeToString(const std::string& str);

std::size_t calcDecodeLength(const std::uint8_t* input, std::size_t len);
void rawBase64Encode(const std::uint8_t* input, std::size_t inputLen, char*& output, std::size_t& outputLen);

template <typename T>
void rawBase64Decode(const std::uint8_t* input, std::size_t inputLen, T& container)
{
  BIO* bio;
  BIO* b64;
  container.resize(calcDecodeLength(input, inputLen));
  
  bio = BIO_new_mem_buf(input, -1);
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_push(b64, bio);
  
  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  int l = BIO_read(bio, std::addressof(container[0]), static_cast<int>(inputLen));
  assert(static_cast<std::size_t>(l) == container.size());
  
  BIO_free_all(bio);
}

template <typename T, typename K>
T encodeInternal(const K& data)
{
  char* ptr = nullptr;
  std::size_t len = 0;
  rawBase64Encode(reinterpret_cast<const std::uint8_t*>(data.data()), data.size(), ptr, len);
  
  T result(ptr, len);
  free(ptr);
  
  return result;
}

template <typename T, typename K>
T decodeInternal(const K& data)
{
  T result;
  rawBase64Decode(reinterpret_cast<const std::uint8_t*>(data.data()), data.size(), result);
  
  return result;
}

} // namespace base64
} // namespace util
} // namespace redi


#endif // REDI_BASE64_HPP
