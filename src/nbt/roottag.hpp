#pragma once

#include "compound.hpp"

namespace redi::nbt {

struct root_tag {
  static constexpr tag_type type = tag_type::type_compound;

  std::string name;
  tag_compound compound;

  tag_value& at(const std::string& key);
  const tag_value& at(const std::string& key) const;

  tag_value& operator[](const std::string& key);
  tag_value& operator[](std::string&& key);

  void visit(const_nbt_visitor& visitor) const;
};

std::ostream& operator<<(std::ostream& stream, const root_tag& root);

} // namespace redi::nbt