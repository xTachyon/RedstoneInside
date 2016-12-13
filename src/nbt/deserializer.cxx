#include <cstdint>
#include <limits>
#include <boost/endian/conversion.hpp>
#include "deserializer.hpp"
#include "proxyclass.hpp"
#include "vectorial.hpp"
#include "exceptions.hpp"

static_assert(sizeof(float) == sizeof(std::int32_t), "sizeof(float) == sizeof(std::int32_t)");
static_assert(sizeof(double) == sizeof(std::int64_t), "sizeof(double) == sizeof(std::int64_t)");
static_assert(std::numeric_limits<float>::is_iec559, "std::numeric_limits<float>::is_iec559");
static_assert(std::numeric_limits<double>::is_iec559, "std::numeric_limits<double>::is_iec559");

namespace endian = boost::endian;

namespace redi
{
namespace nbt
{

NBTDeserializer::NBTDeserializer(const BinaryData& data)
    : mData(data), mOffset(1)
{
  readRoot();
}

void NBTDeserializer::need(std::size_t bytes)
{
  if (mOffset + bytes > mData.size())
    throw RangeErrorException(bytes, mData.size());
}

template <typename T>
T NBTDeserializer::readNumeric()
{
  need(sizeof(T));
  T result;

  std::memcpy(&result, mData.data() + mOffset, sizeof(T));
  mOffset += sizeof(T);

  return endian::big_to_native(result);
}

template <>
float NBTDeserializer::readNumeric<float>()
{
  // is this a good way ?
  std::int32_t r = readNumeric<std::int32_t>();
  return *reinterpret_cast<float*>(&r);
}

template <>
double NBTDeserializer::readNumeric<double>()
{
  // is this a good way ?
  std::int64_t r = readNumeric<std::int64_t>();
  return *reinterpret_cast<double*>(&r);
}


template <typename T>
std::vector<T> NBTDeserializer::readVector()
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

void NBTDeserializer::readRoot()
{
  if (static_cast<NBTType>(mData[0]) != NBTType::Compound) throw InvalidRootTagException();
  root.name = readString();
  readCompound(root);
}

std::string NBTDeserializer::readString()
{
  std::int32_t size = readNumeric<std::int16_t>();
  need(size);
  std::string res(reinterpret_cast<const char*>(mData.data()) + mOffset, size);
  mOffset += size;

  return res;
}

void NBTDeserializer::readCompound(TagCompound& obj)
{
  std::uint8_t type;
  std::string name;

  need(2);
  type = mData[mOffset++];

  while (type != static_cast<std::uint8_t>(NBTType::End))
  {
    if (type > 11) throw InvalidTagTypeException(type);
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

      case NBTType::List:
        obj[name] = TagList();
        readList(obj[name].get().get<NBTType::List>());
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

void NBTDeserializer::readList(TagList& obj)
{
  std::uint8_t type = mData[mOffset++];
  if (type > 11) throw InvalidTagTypeException(type);
  std::int32_t size = readNumeric<std::int32_t>();

  switch (static_cast<NBTType>(type))
  {
    case NBTType::End:
      break;

    case NBTType::Byte:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readNumeric<std::int8_t>());
      break;

    case NBTType::Short:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readNumeric<std::int16_t>());
      break;

    case NBTType::Int:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readNumeric<std::int32_t>());
      break;

    case NBTType::Long:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readNumeric<std::int64_t>());
      break;

    case NBTType::Float:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readNumeric<float>());
      break;

    case NBTType::Double:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readNumeric<double>());
      break;

    case NBTType::ByteArray:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readVector<std::int8_t>());
      break;

    case NBTType::String:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readString());
      break;

    case NBTType::List:
      for (std::int32_t i = 0; i < size; ++i)
      {
        TagPtr lst(new TagList());
        readList(lst->get<NBTType::List>());
        obj.push(lst);
      }
      break;

    case NBTType::Compound:
      for (std::int32_t i = 0; i < size; ++i)
      {
        TagPtr lst(new TagCompound());
        readCompound(lst->get<NBTType::Compound>());
        obj.push(lst);
      }
      break;

    case NBTType::IntArray:
      for (std::int32_t i = 0; i < size; ++i)
        obj.push(readVector<std::int32_t>());
      break;

    default:
      assert(true);
      throw InvalidTagTypeException(type);
  }

}

} // namespace nbt
} // namespace redi