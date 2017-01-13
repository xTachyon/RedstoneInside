#ifndef REDI_NBT_SERIALIZER
#define REDI_NBT_SERIALIZER

#include <boost/endian/conversion.hpp>
#include "roottag.hpp"
#include "../bytebuffer.hpp"

namespace redi
{
namespace nbt
{

class NBTDeserializer
{
public:

  RootTag root;

  NBTDeserializer(const ByteBuffer& data);

  operator RootTag&() { return root; }
  operator RootTag&&() { return std::move(root); } // hmm ?
  operator const RootTag&() const { return root; }

  RootTag& get() { return root; }
  const RootTag& get() const { return root; }

private:

  const ByteBuffer& mData;
  std::size_t mOffset;

  void need(std::size_t bytes);
  void readRoot();
  void readCompound(TagCompound& obj);
  void readList(TagList& obj);
  std::string readString();
  
  template <typename T>
  T readNumber()
  {
    need(sizeof(T));
    T result;
  
    std::memcpy(&result, mData.data() + mOffset, sizeof(T));
    mOffset += sizeof(T);
  
    return boost::endian::big_to_native(result);
  }
  
  template <typename T>
  std::vector<T> readVector()
  {
    std::int32_t size = readNumber<std::int32_t>();
    need(size * sizeof(T));
    std::vector<T> result;
  
    for (std::int32_t i = 0; i < size; ++i)
      result.push_back(readNumber<T>());
    // Compiler, optimize this for ByteArray
    // Thanks
  
    return result;
  }
};

template <>
inline float NBTDeserializer::readNumber<float>()
{
//  // is this a good way ?
//  std::int32_t r = readNumber<std::int32_t>();
//  return *reinterpret_cast<float*>(&r);
  
  std::int32_t c = readNumber<std::int32_t>();
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(c));
  float x;
  std::copy(ptr, ptr + sizeof(c), reinterpret_cast<std::uint8_t*>(std::addressof(x)));
  return x;
}

template <>
inline double NBTDeserializer::readNumber<double>()
{
//  // is this a good way ?
//  std::int64_t r = readNumber<std::int64_t>();
//  return *reinterpret_cast<double*>(&r);
  
  std::int64_t c = readNumber<std::int64_t>();
  auto ptr = reinterpret_cast<const std::uint8_t*>(std::addressof(c));
  double x;
  std::copy(ptr, ptr + sizeof(c), reinterpret_cast<std::uint8_t*>(std::addressof(x)));
  return x;
}

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_SERIALIZER
