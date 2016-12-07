#ifndef REDI_NBT_SERIALIZER
#define REDI_NBT_SERIALIZER

#include "roottag.hpp"
#include "../binarydata.hpp"

namespace redi
{
namespace nbt
{

class NBTDeserializer
{
public:

  RootTag root;

  NBTDeserializer(const BinaryData& data);

  operator RootTag&() { return root; }
  operator RootTag&&() { return std::move(root); } // hmm ?
  operator const RootTag&() const { return root; }

  RootTag& get() { return root; }
  const RootTag& get() const { return root; }

private:

  const BinaryData& mData;
  std::size_t mOffset;

  void need(std::size_t bytes);
  void readRoot();
  void readCompound(TagCompound& obj);
  void readList(TagList& obj);
  std::string readString();
  template <typename T>
  T readNumeric();
  template <typename T>
  std::vector<T> readVector();
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_SERIALIZER
