#ifndef REDI_NBT_BYTESWRITER
#define REDI_NBT_BYTESWRITER


#include "../bytebuffer.hpp"
#include "roottag.hpp"

namespace redi
{
namespace nbt
{

class NBTSerializer
{
public:
  
  ByteBuffer data;
  
  NBTSerializer(const RootTag& root);
  
  operator ByteBuffer&() { return data; }
  operator const ByteBuffer&() const { return data; }
  
  ByteBuffer& get() { return data; }
  const ByteBuffer& get() const { return data; }
  
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
