#include "tag.hpp"
#include "prettyprinter.hpp"

namespace redi::nbt {

tag::~tag() = default;

bool tag::is_number() const {
  switch (get_type()) {
  case tag_type::type_byte:
  case tag_type::type_short:
  case tag_type::type_int:
  case tag_type::type_long:
  case tag_type::type_float:
  case tag_type::type_double:
    return true;

  default:
    return false;
  }
}

bool tag::is_scalar() const { return is_number() || get_type() == tag_type::type_string; }

bool tag::is_vector() const {
  switch (get_type()) {
  case tag_type::type_byte_array:
  case tag_type::type_int_array:
  case tag_type::type_long_array:
    return true;

  default:
    return false;
  }
}

bool tag::is_container() const {
  switch (get_type()) {
  case tag_type::type_compound:
  case tag_type::type_list:
    return true;

  default:
    return false;
  }
}

std::ostream& operator<<(std::ostream& stream, const tag& tag) {
  pretty_printer p;
  tag.visit(p);

  stream << p.string;

  return stream;
}

} // namespace redi::nbt