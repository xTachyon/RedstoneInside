#ifndef REDI_NBT_LIST
# define REDI_NBT_LIST

# include "type.hpp"
# include "tag.hpp"
# include "compiletimemagic.hpp"

namespace redi
{
namespace nbt
{

class ProxyClass;

class TagList : public Tag
{
public:
  
  using Container = std::vector<TagPtr>;
  using iterator = Container::iterator;
  using constIterator = Container::const_iterator;
  using reverseIterator = Container::reverse_iterator;
  using constReverseIterator = Container::const_reverse_iterator;

  static constexpr NBTType type = NBTType::List;

  TagList();
  TagList(const TagList& other);
  TagList(TagList&& other);

  TagList& operator=(const TagList& other);
  TagList& operator=(TagList&& other);

  ProxyClass operator[](std::size_t index);
  const TagPtr& operator[](std::size_t index) const;

  iterator begin() { return mData.begin(); }
  constIterator begin() const { return mData.begin(); }
  void clear() { mData.clear(); mListType = NBTType::End; }
  TagPtr clone() const override;
  bool empty() const { return mData.empty(); }
  iterator end() { return mData.end(); }
  constIterator end() const { return mData.end(); }
  void push(const Tag& value);
  void push(TagPtr value);
  reverseIterator rbegin() { return mData.rbegin(); }
  constReverseIterator rbegin() const { return mData.rbegin(); }
  reverseIterator rend() { return mData.rend(); }
  constReverseIterator rend() const { return mData.rend(); }
  std::size_t size() const { return mData.size(); }
  void swap(TagList& other) { mData.swap(other.mData); }

private:

  std::vector<TagPtr> mData;
  NBTType mListType;
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_LIST