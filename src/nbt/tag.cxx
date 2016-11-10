#include "tag.hpp"

namespace redi
{
namespace nbt
{

Tag::Tag(NBTType type)
  : mType(type) {}

Tag::~Tag() {}

bool Tag::equals(const Tag& r) const
{
  return false;
}

bool operator==(const Tag& l, const Tag& r)
{
  return l.mType == r.mType && l.equals(r);
}

bool operator!=(const Tag& l, const Tag& r)
{
  return !(l == r);
}

} // namespace nbt
} // namespace redi