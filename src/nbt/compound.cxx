#include "compound.hpp"


namespace redi
{
namespace nbt
{

TagCompound::TagCompound(const TagCompound& other)
{
  *this = other;
}

TagCompound::TagCompound(std::initializer_list<value_type> list) : map(list) {}

Value& TagCompound::operator[](const std::string& key)
{
  return map[key];
}

Value& TagCompound::operator[](std::string&& key)
{
  return map[std::move(key)];
}

TagCompound& TagCompound::operator=(const TagCompound& obj)
{
  map.clear();
  
  for (const auto& index : obj.map)
  {
    map.emplace(std::make_pair(index.first, index.second));
  }
  
  return *this;
}
  
} // namespace nbt
} // namespace redi