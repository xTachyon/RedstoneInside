# include <string>
# include <utility>
# include <vector>
# include "tag.hpp"
# include "proxyclass.hpp"
# include "compound.hpp"

namespace redi
{
namespace nbt
{

TagCompound::TagCompound(const TagCompound& other)
  : TagCompound()
{
  *this = other;
}

ProxyClass TagCompound::operator[](const std::string& index)
{
  return ProxyClass(mData[index]);
}

ProxyClass TagCompound::operator[](std::string&& index)
{
  return ProxyClass(mData[std::move(index)]);
}

TagCompound& TagCompound::operator=(const TagCompound& other)
{
  for (auto& index : other.mData)
    mData[index.first] = index.second->clone();

  return *this;
}

bool TagCompound::operator==(const TagCompound& other)
{
  return mData == other.mData;
}

bool TagCompound::operator!=(const TagCompound& other)
{
  return !(*this == other);
}

TagPtr TagCompound::clone() const
{
  return TagPtr(new TagCompound(*this));
}

void TagCompound::insert(const std::string& index, const Tag& data)
{
  mData[index] = data.clone();
}

void TagCompound::insert(std::string&& index, const Tag& data)
{
  mData[std::move(index)] = data.clone();
}

ProxyClass TagCompound::operator[](std::int32_t index)
{
  // for convenience
  return (*this)[std::to_string(index)];
}

TagCompound::TagCompound(const TagCompound::Container& other)
        : Tag(NBTType::Compound), mData(other) {}

TagCompound::TagCompound(TagCompound::Container&& other)
        : Tag(NBTType::Compound), mData(std::move(other)) {}

} // namespace nbt
} // namespace redi
