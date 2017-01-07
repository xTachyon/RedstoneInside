#ifndef REDI_BASE64_HPP
#define REDI_BASE64_HPP

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include "../bytebuffer.hpp"

namespace redi
{
namespace util
{
namespace base64
{

template <typename T, typename K>
T rawEncode(const K& data)
{
  namespace it = boost::archive::iterators;
  using encoder = it::base64_from_binary<it::transform_width<typename K::const_iterator, 6, 8>>;
  
  std::size_t pad = (3 - data.size() % 3) % 3;
  T result(encoder(data.begin()), encoder(data.end()));
  for (std::size_t i = 0; i < pad; ++i) result.push_back('=');
  
  return result;
}

template <typename T, typename K>
T rawDecode(const K& data)
{
  namespace it = boost::archive::iterators;
  using decoder = it::transform_width<it::binary_from_base64<typename K::const_iterator >, 8, 6>;
  
  std::size_t pad = 0;
  if (data[data.size() - 1] == '=')
  {
    ++pad;
    if (data[data.size() - 2] == '=') ++pad;
  }
  
  T result(decoder(data.begin()), decoder(data.end()));
  result.erase(result.end() - pad, result.end());
  
  return result;
}


ByteBuffer encode(const ByteBuffer& data);
ByteBuffer encode(const std::string& str);
std::string encodeToString(const ByteBuffer& data);
std::string encodeToString(const std::string& str);

ByteBuffer decode(const ByteBuffer& data);
ByteBuffer decode(const std::string& str);
std::string decodeToString(const ByteBuffer& data);
std::string decodeToString(const std::string& str);

} // namespace base64
} // namespace util
} // namespace redi

#endif // REDI_BASE64_HPP