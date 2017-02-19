#ifndef REDI_NBT_LIST_HPP
#define REDI_NBT_LIST_HPP

#include <vector>
#include "basic.hpp"

namespace redi
{
namespace nbt
{

class TagList : public BasicTag<TagList>
{
public:
  
  static constexpr Type type = Type::List;
  
  TagList() = default;
  TagList(const TagList& other);
  TagList(TagList&& other) = default;
  
  TagList& operator=(const TagList& other);
  TagList& operator=(TagList&& other) = default;
  
  Value& operator[](std::size_t pos) { return data[pos]; }
  const Value& operator[](std::size_t pos) const { return data[pos]; }
  
  void push_back(const Value& value) { data.push_back(value); }
  void push_back(Value&& value) { data.push_back(std::move(value)); }
  
  void pop_back() { data.pop_back(); }
  
  std::size_t size() const { return data.size(); }
  bool empty() const { return data.empty(); }
  
  void write(Serializer& s) const override;
  void read(Deserializer& s) override;
  Type getListType() const;
  
private:
  
  std::vector<Value> data;
};
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_LIST_HPP