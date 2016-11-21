#include "list.hpp"
#include "proxyclass.hpp"

namespace redi
{
namespace nbt
{

TagList::TagList()
  : Tag(NBTType::List), mListType(NBTType::End) {}

TagList::TagList(const TagList& other)
  : TagList()
{
  *this = other;
}

TagList& TagList::operator=(const TagList& other)
{
  mData.clear();
  mListType = other.mListType;
  mData.reserve(other.mData.size());
  for (const auto& index : other.mData)
    mData.push_back(index->clone());

  return *this;
}

TagList& TagList::operator=(TagList&& other)
{
  mListType = other.mListType;
  mData = std::move(other.mData);

  return *this;
}

ProxyClass TagList::operator[](std::size_t index)
{
  return ProxyClass(mData[index]);
}

const TagPtr& TagList::operator[](std::size_t index) const
{
  return mData[index];
}

TagPtr TagList::clone() const
{
  return TagPtr(new TagList(*this));
}

void TagList::push(const Tag& value)
{
  if (value.getType() == mListType)
    mData.push_back(value.clone());
  else if (mListType == NBTType::End || empty())
  {
    mData.push_back(value.clone());
    mListType = value.getType();
  }
  // TODO: thrown an exception here
}

void TagList::push(TagPtr value)
{
  if (value->getType() == mListType && value)
    mData.push_back(value);
  else if (mListType == NBTType::End || empty())
  {
    mData.push_back(value);
    mListType = value->getType();
  }
  // and here
}

} // namespace nbt
} // namespace redi
