#include "tag.hpp"

namespace redi
{
namespace nbt
{

Tag::Tag(NBTType type)
  : mType(type) {}

Tag::~Tag() {}

bool operator==(const Tag& l, const Tag& r)
{
  return l.mType == r.mType && l.equals(r);
}

bool Tag::isScalar() const
{
  switch (mType)
  {
    case NBTType::Byte:
    case NBTType::Short:
    case NBTType::Int:
    case NBTType::Long:
    case NBTType::Float:
    case NBTType::Double:
    case NBTType::String:
      return true;

    default:
      return false;
  }
}

bool Tag::isVector() const
{
  switch (mType)
  {
    case NBTType::ByteArray:
    case NBTType::IntArray:
      return true;

    default:
      return false;
  }
}

bool Tag::isComplex() const
{
  switch (mType)
  {
    case NBTType::Compound:
    case NBTType::List:
      return true;

    default:
      return false;
  }
}

bool operator!=(const Tag& l, const Tag& r)
{
  return !(l == r);
}

} // namespace nbt
} // namespace redi