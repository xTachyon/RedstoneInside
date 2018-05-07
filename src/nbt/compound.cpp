#include "serializer.hpp"
#include "deserializer.hpp"
#include "creator.hpp"
#include "compound.hpp"

namespace redi::nbt {

tag_compound::tag_compound(const tag_compound& other) { *this = other; }

tag_compound::tag_compound(std::initializer_list<value_type> list) : map(list) {}

tag_value& tag_compound::operator[](const std::string& key) { return map[key]; }

tag_value& tag_compound::operator[](std::string&& key) {
  return map[std::move(key)];
}

tag_compound& tag_compound::operator=(const tag_compound& obj) {
  map.clear();

  for (const auto& index : obj.map) {
    map.emplace(std::make_pair(index.first, index.second));
  }

  return *this;
}

} // namespace redi::nbt