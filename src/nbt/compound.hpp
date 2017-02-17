#ifndef REDI_NBT_COMPOUND_HPP
#define REDI_NBT_COMPOUND_HPP

#include <map>
#include "basic.hpp"
#include "value.hpp"

namespace redi
{
namespace nbt
{

class TagCompound : public BasicTag<TagCompound>
{
public:
  
  using MapType = std::map<std::string, Value>;
  
  using reference = MapType::reference;
  using const_reference = MapType::const_reference;
  using iterator = MapType::iterator;
  using const_iterator = MapType::const_iterator;
  using difference_type = MapType::difference_type;
  using size_type = MapType::size_type;
  using value_type = MapType::value_type;
  
  using reverse_iterator = MapType::reverse_iterator;
  using const_reverse_iterator = MapType::const_reverse_iterator;
  
  static constexpr Type type = Type::Compound;
  
  TagCompound() = default;
  TagCompound(const TagCompound& other);
  TagCompound(TagCompound&&) = default;
  TagCompound(std::initializer_list<value_type> list);
  
  TagCompound& operator=(const TagCompound&);
  TagCompound& operator=(TagCompound&&) = default;
  
  Value& operator[](const std::string& key);
  Value& operator[](std::string&& key);
  
  Value& at(const std::string& key) { return map.at(key); }
  const Value& at(const std::string& key) const { return map.at(key); }
  
  void clear() { map.clear(); }
  std::size_t size() const { return map.size(); }
  bool empty() const { return map.empty(); }
  void swap(TagCompound& other) { map.swap(other.map); }
  
  template <typename ... Args>
  std::pair<iterator, bool> emplace(Args&&... args)
  {
    return map.emplace(std::forward<Args>(args)...);
  }
  
  iterator begin() { return map.begin(); }
  const_iterator begin() const { return map.begin(); }
  const_iterator cbegin() const { return map.cbegin(); }
  
  iterator end() { return map.end(); }
  const_iterator end() const { return map.end(); }
  const_iterator cend() const { return map.cend(); }
  
  reverse_iterator rbegin() { return map.rbegin(); }
  const_reverse_iterator rbegin() const { return map.rbegin(); }
  const_reverse_iterator crbegin() const { return map.crbegin(); }
  
  reverse_iterator rend() { return map.rend(); }
  const_reverse_iterator rend() const { return map.rend(); }
  const_reverse_iterator crend() const { return map.crend(); }
  
private:
  
  MapType map;
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_COMPOUND_HPP