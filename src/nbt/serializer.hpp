#ifndef REDI_NBT_SERIALIZER_HPP
#define REDI_NBT_SERIALIZER_HPP

#include <boost/endian/conversion.hpp>
#include "../bytebuffer.hpp"
#include "../util/util.hpp"
#include "forward.hpp"
#include "type.hpp"

namespace redi
{
namespace nbt
{

struct Serializer
{
  ByteBuffer& buffer;
  
  Serializer(ByteBuffer& buffer);
  
  ByteBuffer& operator()(const std::string& name, const TagCompound& root);
  
  template <typename T>
  void writeNumber(T x)
  {
    boost::endian::native_to_big_inplace(x);
    buffer.append(x);
  }
  
  void writeString(const std::string& str);
  void writeType(const Value& value);
  void writeType(Type type);
  void write(const std::string& name, const TagCompound& root);
};

template <>
inline void Serializer::writeNumber<float>(float x)
{
  writeNumber(util::binaryTo<float, std::int32_t>(x));
}

template <>
inline void Serializer::writeNumber<double>(double x)
{
  writeNumber(util::binaryTo<double, std::int64_t>(x));
}

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_SERIALIZER_HPP