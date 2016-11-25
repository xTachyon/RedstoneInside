#ifndef REDI_NBT_COMPOUND
#	define REDI_NBT_COMPOUND

# include <map>
# include <string>
# include <utility>
# include "tag.hpp"

namespace redi
{
namespace nbt
{

class ProxyClass;

class TagCompound : public Tag
{
public:

  using Container = std::map<std::string, TagPtr>;
  using iterator = Container::iterator;
  using constIterator = Container::const_iterator;
  using reverseIterator = Container::reverse_iterator;
  using constReverseIterator = Container::const_reverse_iterator;

  static constexpr NBTType type = NBTType::Compound;

  TagCompound()
    : Tag(NBTType::Compound) {}

  TagCompound(const TagCompound& other);

  ProxyClass operator[](const std::string& index);
  ProxyClass operator[](std::string&& index);

  TagCompound& operator=(const TagCompound& other);

  bool operator==(const TagCompound& other);
  bool operator!=(const TagCompound& other);

  iterator begin() { return mData.begin(); }
  constIterator begin() const { return mData.begin(); }
  void clear() { mData.clear(); }
  TagPtr clone() const override;
  bool empty() const { return mData.empty(); }
  iterator end() { return mData.end(); }
  constIterator end() const { return mData.end(); }
  void insert(const std::string& index, const Tag& data);
  void insert(std::string&& index, const Tag& data);
  reverseIterator rbegin() { return mData.rbegin(); }
  constReverseIterator rbegin() const { return mData.rbegin(); }
  reverseIterator rend() { return mData.rend(); }
  constReverseIterator rend() const { return mData.rend(); }
  std::size_t size() const { return mData.size(); }
  void swap(TagCompound& other) { mData.swap(other.mData); }

protected:

  bool equals(const Tag& r) const override
  {
    return *this == dynamic_cast<const TagCompound&>(r);
  }

private:

  Container mData;
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_COMPOUND