#include "primitive.hpp"
#include "string.hpp"
#include "array.hpp"
#include "compound.hpp"
#include "list.hpp"
#include "creator.hpp"

namespace redi {
namespace nbt {

TagUniquePtr create(Type t) {
  TagUniquePtr ptr;

  switch (t) {
  case Type::End: {
    ptr = nullptr;
  } break;

  case Type::Byte: {
    ptr = std::make_unique<TagByte>();
  } break;

  case Type::Short: {
    ptr = std::make_unique<TagShort>();
  } break;

  case Type::Int: {
    ptr = std::make_unique<TagInt>();
  } break;

  case Type::Long: {
    ptr = std::make_unique<TagLong>();
  } break;

  case Type::Float: {
    ptr = std::make_unique<TagFloat>();
  } break;

  case Type::Double: {
    ptr = std::make_unique<TagDouble>();
  } break;

  case Type::ByteArray: {
    ptr = std::make_unique<TagByteArray>();
  } break;

  case Type::String: {
    ptr = std::make_unique<TagString>();
  } break;

  case Type::List: {
    ptr = std::make_unique<TagList>();
  } break;

  case Type::Compound: {
    ptr = std::make_unique<TagCompound>();
  } break;

  case Type::IntArray: {
    ptr = std::make_unique<TagIntArray>();
  } break;

  case Type::ShortArray: {
    ptr = std::make_unique<TagShortArray>();
  } break;
  }

  return ptr;
}

Value createValue(Type t) { return Value(create(t)); }

} // namespace nbt
} // namespace redi