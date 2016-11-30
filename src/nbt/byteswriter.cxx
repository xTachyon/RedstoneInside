#include <boost/endian/conversion.hpp>
#include "byteswriter.hpp"
#include "nbt.hpp"
#include <fstream>

namespace redi
{
namespace nbt
{

BytesWriter::BytesWriter(const RootTag& root)
      : mRoot(root)
{
  writeRoot();
  // std::ofstream("out").write(reinterpret_cast<const char*>(data.c_str()), data.size());
}

template <typename T>
void BytesWriter::writeNumeric(T number)
{
  boost::endian::native_to_big_inplace(number);
  writeRaw(number);
}

template <>
void BytesWriter::writeNumeric<float>(float number)
{
  writeNumeric(*reinterpret_cast<std::int32_t*>(&number));
}

template <>
void BytesWriter::writeNumeric<double>(double number)
{
  writeNumeric(*reinterpret_cast<std::int64_t*>(&number));
}

void BytesWriter::writeString(const std::string& str)
{
  writeNumeric(static_cast<std::int16_t>(str.size()));
  writeRaw(str.c_str(), str.size());
}

template <typename T>
void BytesWriter::writeVector(const std::vector<T>& vect)
{
  writeNumeric(static_cast<std::int32_t>(vect.size()));
  
  for (std::size_t i = 0; i < vect.size(); ++i)
    writeRaw<T>(vect[i]);
}

void BytesWriter::writeCompound(const TagCompound& obj)
{
  for (const auto& index : obj)
  {
    const Tag& ref = *index.second;
    data.push_back(static_cast<std::uint8_t>(ref.getType()));
    writeString(index.first);
    
    switch (ref.getType())
    {
      case NBTType::Byte:
        writeNumeric<std::int8_t>(ref.get<NBTType::Byte>());
        break;
        
      case NBTType::Short:
        writeNumeric<std::int16_t>(ref.get<NBTType::Short>());
        break;
  
      case NBTType::Int:
        writeNumeric<std::int32_t>(ref.get<NBTType::Int>());
        break;
  
      case NBTType::Long:
        writeNumeric<std::int64_t>(ref.get<NBTType::Long>());
        break;
  
      case NBTType::Float:
        writeNumeric<float>(ref.get<NBTType::Float>());
        break;
  
      case NBTType::Double:
        writeNumeric<double>(ref.get<NBTType::Double>());
        break;
  
      case NBTType::ByteArray:
        writeVector(ref.get<NBTType::ByteArray>());
        break;
  
      case NBTType::String:
        writeString(ref.get<NBTType::String>());
        break;
        
      case NBTType::List:
        writeList(ref.get<NBTType::List>());
        break;
  
      case NBTType::Compound:
        writeCompound(ref.get<NBTType::Compound>());
        break;
  
      case NBTType::IntArray:
        writeVector(ref.get<NBTType::IntArray>());
        break;
    }
  }
  
  data.push_back(static_cast<std::uint8_t>(NBTType::End));
}

void BytesWriter::writeList(const TagList& obj)
{
  data.push_back(static_cast<std::uint8_t>(obj.getListType()));
  writeNumeric(static_cast<std::int32_t>(obj.size()));
  
  switch (obj.getListType())
  {
    case NBTType::Byte:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeNumeric<std::int8_t>(obj[i]->get<NBTType::Byte>());
      break;
    
    case NBTType::Short:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeNumeric<std::int16_t>(obj[i]->get<NBTType::Short>());
      break;
    
    case NBTType::Int:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeNumeric<std::int32_t>(obj[i]->get<NBTType::Int>());
      break;
      
    case NBTType::Long:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeNumeric<std::int64_t>(obj[i]->get<NBTType::Long>());
      break;
    
    case NBTType::Float:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeNumeric<float>(obj[i]->get<NBTType::Float>());
      break;
    
    case NBTType::Double:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeNumeric<double>(obj[i]->get<NBTType::Double>());
      break;
    
    case NBTType::ByteArray:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeVector(obj[i]->get<NBTType::ByteArray>());
      break;
    
    case NBTType::String:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeString(obj[i]->get<NBTType::String>());
      break;
    
    case NBTType::List:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeList(obj[i]->get<NBTType::List>());
      break;
    
    case NBTType::Compound:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeCompound(obj[i]->get<NBTType::Compound>());
      break;
    
    case NBTType::IntArray:
      for (std::size_t i = 0; i < obj.size(); ++i)
        writeVector(obj[i]->get<NBTType::IntArray>());
      break;
  }
}

void BytesWriter::writeRoot()
{
  data.push_back(static_cast<std::uint8_t>(NBTType::Compound));
  writeString(mRoot.name);
  writeCompound(mRoot);
}
  
} // namespace nbt
} // namespace redi

#pragma clang diagnostic pop