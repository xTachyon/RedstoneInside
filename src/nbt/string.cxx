#include "string.hpp"

namespace redi
{
namespace nbt
{

bool operator==(const TagString& l, const TagString& r)
{
  return l.data == r.data;
}

bool operator!=(const TagString& l, const TagString& r)
{
  return l.data != r.data;
}

TagString::TagString()
  : Tag(type) {}

TagString::TagString(const char* ptr)
  : TagString()
{
  data = ptr;
}

TagString::TagString(const std::string& ref)
  : TagString()
{
  data = ref;
}

TagString::TagString(std::string&& ref)
  : TagString()
{
  data = std::move(ref);
}

TagPtr TagString::clone() const
{
  return TagPtr(new TagString(*this));
}

} // namespace nbt
} // namespace redi