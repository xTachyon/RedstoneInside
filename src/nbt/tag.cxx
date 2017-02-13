#include "tag.hpp"

namespace redi
{
namespace nbt
{

Tag::~Tag() {}

bool Tag::isNumber() const
{
  switch (getType())
  {
    case Type::Byte:
    case Type::Short:
    case Type::Int:
    case Type::Long:
    case Type::Float:
    case Type::Double:
      return true;

    default:
      return false;
  }
}

bool Tag::isScalar() const
{
  return isNumber() || getType() == Type::String;
}

bool Tag::isVector() const
{
  switch (getType())
  {
    case Type::ByteArray:
    case Type::IntArray:
      return true;

    default:
      return false;
  }
}

bool Tag::isContainer() const
{
  switch (getType())
  {
    case Type::Compound:
    case Type::List:
      return true;

    default:
      return false;
  }
}

} // namespace nbt
} // namespace redi