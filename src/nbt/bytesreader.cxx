#include <limits>
#include <boost/endian/conversion.hpp>
#include "bytesreader.hpp"
#include "proxyclass.hpp"
#include "vectorial.hpp"
#include "exceptions.hpp"

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
      : mData(data), mOffset(1)
{
  readRoot();
}

void BytesReader::need(std::size_t bytes)
{
  if (mOffset + bytes > mData.size())
    throw RangeErrorException(bytes, mData.size());
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
double BytesReader::readNumeric<double>()
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
  if (static_cast<NBTType>(mData[0]) != NBTType::Compound) throw InvalidRootTagException();
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
  NBTType type;
  std::string name;
  
  need(2);
  type = static_cast<NBTType>(mData[mOffset++]);
  
  while (type != NBTType::End)
  {
    name = readString();
    
    switch (type)
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
    
    type = static_cast<NBTType>(mData[mOffset++]);
  }
}

void BytesReader::readList(TagList& obj)
{
  NBTType type = static_cast<NBTType>(mData[mOffset++]);
  std::int32_t size = readNumeric<std::int32_t>();
  
  switch (type)
  {
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