#include "base64.hpp"


namespace redi
{
namespace util
{
namespace base64
{

ByteBuffer encode(const ByteBuffer& data)
{
  return rawEncode<ByteBuffer, ByteBuffer>(data);
}

ByteBuffer encode(const std::string& str)
{
  return rawEncode<ByteBuffer, std::string>(str);
}

std::string encodeToString(const std::string& str)
{
  return rawEncode<std::string, std::string>(str);
}

std::string encodeToString(const ByteBuffer& data)
{
  return rawEncode<std::string, ByteBuffer>(data);
}

ByteBuffer decode(const ByteBuffer& data)
{
  return rawDecode<ByteBuffer, ByteBuffer>(data);
}

ByteBuffer decode(const std::string& str)
{
  return rawDecode<ByteBuffer, std::string>(str);
}

std::string decodeToString(const ByteBuffer& data)
{
  return rawDecode<std::string, ByteBuffer>(data);
}

std::string decodeToString(const std::string& str)
{
  return rawDecode<std::string, std::string>(str);
}
  
} // namespace base64
} // namespace util
} // namespace redi