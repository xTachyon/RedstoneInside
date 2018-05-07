#include "roottag.hpp"

namespace redi::nbt {

void root_tag::visit(const_nbt_visitor& visitor) const {
  visitor.visit(*this);
}

tag_value& root_tag::at(const std::string& key) {
  return compound.at(key);
}

const tag_value& root_tag::at(const std::string& key) const {
  return compound.at(key);
}

tag_value& root_tag::operator[](const std::string& key) {
  return compound[key];
}

tag_value& root_tag::operator[](std::string&& key) {
  return compound[std::move(key)];
}

std::ostream& operator<<(std::ostream& stream, const root_tag& root) {
  pretty_printer printer;
  printer.visit(root);

  stream << printer.string;

  return stream;
}

} // namespace redi::nbt
