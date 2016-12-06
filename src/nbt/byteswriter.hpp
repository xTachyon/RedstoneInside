#ifndef REDI_NBT_BYTESWRITER
#define REDI_NBT_BYTESWRITER


#include "../binarydata.hpp"
#include "roottag.hpp"

namespace redi
{
namespace nbt
{

class BytesWriter
{
public:
  
  BinaryData data;
  
  BytesWriter(const RootTag& root);
  
  operator BinaryData&() { return data; }
  operator const BinaryData&() const { return data; }
  
  BinaryData& get() { return data; }
  const BinaryData& get() const { return data; }
  
private:
  
  const RootTag& mRoot;
  
  template <typename T>
  void writeNumeric(T number);
  template <typename T>
  void writeVector(const std::vector<T>& vect);
  
  template <typename T>
  void writeRaw(const T& val, std::size_t size = sizeof(T))
  {
    data.append(reinterpret_cast<const std::uint8_t*>(&val), size);
  }
  
  void writeRaw(const char* ptr, std::size_t size)
  {
    data.append(reinterpret_cast<const std::uint8_t*>(ptr), size);
  }
  
  void writeString(const std::string& str);
  void writeCompound(const TagCompound& obj);
  void writeList(const TagList& obj);
  void writeRoot();
};

} // namespace nbt
} // namespace redi


#endif // REDI_NBT_BYTESWRITER
