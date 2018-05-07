#include "primitive.hpp"
#include "string.hpp"
#include "array.hpp"
#include "compound.hpp"
#include "list.hpp"
#include "creator.hpp"
#include "end.hpp"

namespace redi::nbt {

tag_unique_ptr create(tag_type t) {
  tag_unique_ptr ptr;

  switch (t) {
  case tag_type::type_end: {
    ptr = std::make_unique<tag_end>();
  } break;

  case tag_type::type_byte: {
    ptr = std::make_unique<tag_byte>();
  } break;

  case tag_type::type_short: {
    ptr = std::make_unique<tag_short>();
  } break;

  case tag_type::type_int: {
    ptr = std::make_unique<tag_int>();
  } break;

  case tag_type::type_long: {
    ptr = std::make_unique<tag_long>();
  } break;

  case tag_type::type_float: {
    ptr = std::make_unique<tag_float>();
  } break;

  case tag_type::type_double: {
    ptr = std::make_unique<tag_double>();
  } break;

  case tag_type::type_byte_array: {
    ptr = std::make_unique<tag_byte_array>();
  } break;

  case tag_type::type_string: {
    ptr = std::make_unique<tag_string>();
  } break;

  case tag_type::type_list: {
    ptr = std::make_unique<tag_list>();
  } break;

  case tag_type::type_compound: {
    ptr = std::make_unique<tag_compound>();
  } break;

  case tag_type::type_int_array: {
    ptr = std::make_unique<tag_int_array>();
  } break;

  case tag_type::type_long_array: {
    ptr = std::make_unique<tag_long_array>();
  } break;
  }

  return ptr;
}

tag_value createValue(tag_type t) { return tag_value(create(t)); }

} // namespace redi::nbt