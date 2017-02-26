#ifndef REDI_NBT_DESERIALIZER_HPP
#define REDI_NBT_DESERIALIZER_HPP

#include <boost/endian/conversion.hpp>
#include "../bytebuffer.hpp"
//#include "../util/util.hpp"
#include "type.hpp"

//#include "../util/base64.hpp"
//#include "../util/file.hpp"
#include "../util/string.hpp"
//#include "../util/time.hpp"
//#include "../util/maths.hpp"
//#include "../util/random.hpp"

namespace redi
{
namespace nbt
{

struct Deserializer
{
  const ByteBuffer& buffer;
  std::size_t offset;
  
  Deserializer(const ByteBuffer& buffer, std::size_t offset = 0);
  
  void operator()(std::string& name, TagCompound& root);
  void operator()(TagCompound& root);
  
  template <typename T>
  T readNumber()
  {
    need(sizeof(T));
    T x;
    
    std::copy(buffer.data() + offset, buffer.data() + offset + sizeof(T),
              reinterpret_cast<std::uint8_t*>(std::addressof(x)));
    offset += sizeof(T);
    return boost::endian::big_to_native(x);
  }
  
  std::string readString();
  Type readType();
  void read(std::string& name, TagCompound& root);
  void read(TagCompound& root);
  void read(RootTag& root);
  
  void need(std::size_t bytes);
};

template <>
inline float Deserializer::readNumber<float>()
{
  std::int32_t x = readNumber<std::int32_t>();
  return util::binaryTo<std::int32_t, float>(x);
}

template <>
inline double Deserializer::readNumber<double>()
{
  std::int64_t x = readNumber<std::int64_t>();
  return util::binaryTo<std::int64_t, double>(x);
}

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_DESERIALIZER_HPP