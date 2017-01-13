#include "type.hpp"

namespace redi
{
namespace nbt
{
const char* getNBTTypeName(NBTType type)
{
  const char* ptr;

  switch (type)
  {
  case NBTType::End:
    ptr = "TAG_End";
    break;

  case NBTType::Byte:
    ptr = "TAG_Byte";
    break;

  case NBTType::Short:
    ptr = "TAG_Short";
    break;

  case NBTType::Int:
    ptr = "TAG_Int";
    break;

  case NBTType::Long:
    ptr = "TAG_Long";
    break;

  case NBTType::Float:
    ptr = "TAG_Float";
    break;

  case NBTType::Double:
    ptr = "TAG_Double";
    break;

  case NBTType::ByteArray:
    ptr = "TAG_Byte_Array";
    break;

  case NBTType::String:
    ptr = "TAG_String";
    break;

  case NBTType::List:
    ptr = "TAG_List";
    break;

  case NBTType::Compound:
    ptr = "TAG_Compound";
    break;

  case NBTType::IntArray:
    ptr = "TAG_Int_Array";
    break;

  default:
    ptr = "unknown; if you see this, something went TERRIBLY wrong";
  }

  return ptr;
}

} // namespace nbt
} // namespace redi