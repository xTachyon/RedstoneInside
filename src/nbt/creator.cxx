#include "creator.hpp"
#include "scalar.hpp"
#include "compound.hpp"

namespace redi
{
namespace nbt
{

TagPtr create(NBTType type)
{
  switch (type)
  {
  case redi::nbt::NBTType::End:
    break;

  case redi::nbt::NBTType::Byte:
    return TagPtr(new TagByte());

  case redi::nbt::NBTType::Short:
    return TagPtr(new TagShort());

  case redi::nbt::NBTType::Int:
    return TagPtr(new TagInt());

  case redi::nbt::NBTType::Long:
    return TagPtr(new TagLong());

  case redi::nbt::NBTType::Float:
    return TagPtr(new TagFloat());

  case redi::nbt::NBTType::Double:
    return TagPtr(new TagDouble());

  case redi::nbt::NBTType::ByteArray:
    break;

  case redi::nbt::NBTType::String:
    break;

  case redi::nbt::NBTType::List:
    break;

  case redi::nbt::NBTType::Compound:
    return TagPtr(new TagCompound());

  case redi::nbt::NBTType::IntArray:
    break;

  default:
    return nullptr;
  }

  return nullptr;
}

} // namespace nbt
} // namespace redi
