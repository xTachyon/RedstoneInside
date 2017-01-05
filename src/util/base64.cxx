#include <cassert>
#include <string>
#include <vector>
#include "base64.hpp"

namespace redi
{
namespace util
{
namespace base64
{

void rawBase64Encode(const std::uint8_t* input, std::size_t inputLen, char*& output, std::size_t& outputLen)
{
  BIO* bio;
  BIO* b64;
  BUF_MEM* bufferPtr;
  
  b64 = BIO_new(BIO_f_base64());
  bio = BIO_new(BIO_s_mem());
  bio = BIO_push(b64, bio);
  
  BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
  BIO_write(bio, input, static_cast<int>(inputLen));
  BIO_flush(bio);
  BIO_get_mem_ptr(bio, std::addressof(bufferPtr));
  BIO_set_close(bio, BIO_NOCLOSE);
  
  output = bufferPtr->data;
  outputLen = bufferPtr->length;
  free(bufferPtr);
  
  BIO_free_all(bio);
}

std::size_t calcDecodeLength(const std::uint8_t* input, std::size_t len)
{
  std::size_t padding = 0;
  
  if (input[len - 1] == '=')
  {
    ++padding;
    if (input[len - 2] == '=') ++padding;
  }
  
  return len * 3 / 4 - padding;
}

ByteBuffer encode(const ByteBuffer& data)
{
  return encodeInternal<ByteBuffer, ByteBuffer>(data);
}

ByteBuffer encode(const std::string& str)
{
  return encodeInternal<ByteBuffer, std::string>(str);
}

std::string encodeToString(const std::string& str)
{
  return encodeInternal<std::string, std::string>(str);
}

ByteBuffer decode(const ByteBuffer& data)
{
  return decodeInternal<ByteBuffer, ByteBuffer>(data);
}

ByteBuffer decode(const std::string& str)
{
  return decodeInternal<ByteBuffer, std::string>(str);
}

std::string decodeToString(const ByteBuffer& data)
{
  return decodeInternal<std::string, ByteBuffer>(data);
}

std::string decodeToString(const std::string& str)
{
  return decodeInternal<std::string, std::string>(str);
}
  
} // namespace base64
} // namespace util
} // namespace redi