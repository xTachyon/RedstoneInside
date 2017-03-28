#include "tag.hpp"
#include "prettyprinter.hpp"

namespace redi {
namespace nbt {

Tag::~Tag() {}

bool Tag::isNumber() const {
  switch (getType()) {
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

bool Tag::isScalar() const { return isNumber() || getType() == Type::String; }

bool Tag::isVector() const {
  switch (getType()) {
  case Type::ByteArray:
  case Type::IntArray:
    return true;

  default:
    return false;
  }
}

bool Tag::isContainer() const {
  switch (getType()) {
  case Type::Compound:
  case Type::List:
    return true;

  default:
    return false;
  }
}

std::ostream& operator<<(std::ostream& stream, const Tag& tag) {
  PrettyPrint p;
  tag.writePretty(p);

  stream << p.string;

  return stream;
}

} // namespace nbt
} // namespace redi