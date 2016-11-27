#include <limits>
#include <boost/endian/conversion.hpp>
#include "bytesreader.hpp"
#include "proxyclass.hpp"
#include "vectorial.hpp"

static_assert(sizeof(float) == sizeof(std::int32_t));
static_assert(sizeof(double) == sizeof(std::int64_t));
static_assert(std::numeric_limits<float>::is_iec559);
static_assert(std::numeric_limits<double>::is_iec559);

namespace endian = boost::endian;

namespace redi
{
namespace nbt
{

BytesReader::BytesReader(const BinaryData& data)
        : mData(data), mOffset(0)
{
  readRoot();
}

void BytesReader::need(std::size_t bytes)
{
  if (mOffset + bytes > mData.size())
    throw std::invalid_argument(std::to_string(bytes) + " is out of index");
}

template <typename T>
T BytesReader::readNumeric()
{
  need(sizeof(T));
  T result;

  std::memcpy(&result, mData.data() + mOffset, sizeof(T));
  mOffset += sizeof(T);

  return endian::big_to_native(result);
}

template <>
float BytesReader::readNumeric<float>()
{
  // is this a good way ?
  std::int32_t r = readNumeric<std::int32_t>();
  return *reinterpret_cast<float*>(&r);
}

template <>
double BytesReader::readNumeric<double >()
{
  // is this a good way ?
  std::int64_t r = readNumeric<std::int64_t>();
  return *reinterpret_cast<double*>(&r);
}


template <typename T>
std::vector<T> BytesReader::readVector()
{
  std::int32_t size = readNumeric<std::int32_t>();
  need(size * sizeof(T));
  std::vector<T> result;

  for (std::int32_t i = 0; i < size; ++i)
    result.push_back(readNumeric<T>());
  // Compiler, optimize this for ByteArray
  // Thanks

  return result;
}

void BytesReader::readRoot()
{
  if (static_cast<NBTType>(mData[0]) != NBTType::Compound) throw std::invalid_argument("root tag must pe compound");
  mOffset = 1;
  root.name = readString();
  readCompound(root);
}

std::string BytesReader::readString()
{
  std::int32_t size = readNumeric<std::int16_t>();
  need(size);
  std::string res(reinterpret_cast<const char*>(mData.data()) + mOffset, size);
  mOffset += size;

  return res;
}

void BytesReader::readCompound(TagCompound& obj)
{
  std::int8_t type;
  std::string name;

  need(2);
  type = mData[mOffset++];

  while (static_cast<NBTType>(type) != NBTType::End)
  {
    name = readString();

    switch (static_cast<NBTType>(type))
    {
      case NBTType::Byte:
        obj[name] = readNumeric<std::int8_t>();
        break;

      case NBTType::Short:
        obj[name] = readNumeric<std::int16_t>();
        break;

      case NBTType::Int:
        obj[name] = readNumeric<std::int32_t>();
        break;

      case NBTType::Long:
        obj[name] = readNumeric<std::int64_t>();
        break;

      case NBTType::Float:
        obj[name] = readNumeric<float>();
        break;

      case NBTType::Double:
        obj[name] = readNumeric<double>();
        break;

      case NBTType::ByteArray:
        obj[name] = readVector<std::int8_t>();
        break;

      case NBTType::String:
        obj[name] = readString();
        break;

      case NBTType::Compound:
        obj[name] = TagCompound();
        readCompound(obj[name].get().get<NBTType::Compound>());
        break;

      case NBTType::IntArray:
        obj[name] = readVector<std::int32_t>();
        break;

      default:
        throw std::invalid_argument("invalid nbt type");
    }

    type = mData[mOffset++];
  }
}

} // namespace nbt
} // namespace redi