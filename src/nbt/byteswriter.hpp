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
  
private:
  
  const RootTag& mRoot;
};

} // namespace nbt
} // namespace redi


#endif // REDI_NBT_BYTESWRITER
