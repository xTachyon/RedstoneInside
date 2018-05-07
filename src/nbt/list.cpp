#include "value.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "creator.hpp"
#include "list.hpp"

namespace redi::nbt {

namespace {

std::vector<tag_value> cloneVectorValue(const std::vector<tag_value>& vector) {
  std::vector<tag_value> result;
  result.reserve(vector.size());
  for (auto& i : vector) {
    result.push_back(i);
  }

  return result;
}

}

tag_list::tag_list(const tag_list& other) { *this = other; }

tag_list& tag_list::operator=(const tag_list& other) {
  data = cloneVectorValue(other.data);

  return *this;
}

tag_type tag_list::getListType() const {
  return empty() ? tag_type::type_end : data.front().get_type();
}

} // namespace redi::nbt