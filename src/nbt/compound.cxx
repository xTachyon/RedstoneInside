#include "serializer.hpp"
#include "deserializer.hpp"
#include "creator.hpp"
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

void TagCompound::write(Serializer& s) const
{
  for (const auto& index : map)
  {
    Type t = index.second.getType();
  
    s.writeType(t);
    s.writeString(index.first);
    if (t != Type::End)
    {
      index.second.write(s);
    }
  }
  
  s.writeType(Type::End);
}

void TagCompound::read(Deserializer& s)
{
  Type t;
  std::string name;
  
  while ((t = s.readType()) != Type::End)
  {
    name = s.readString();
    TagUniquePtr tag = create(t);
    if (t != Type::End)
    {
      tag->read(s);
    }
    map[std::move(name)] = std::move(tag);
  }
}
  
} // namespace nbt
} // namespace redi