#ifndef REDI_NBT_STRING
# define REDI_NBT_STRING

# include "type.hpp"
# include "tag.hpp"

namespace redi
{
namespace nbt
{

class TagString : public Tag
{
public:

  static constexpr NBTType type = NBTType::String;

  std::string data;

  TagString();
  TagString(const char* ptr);
  TagString(const std::string& ref);
  TagString(std::string&& ref);

  operator std::string&() { return data; }
  operator const std::string&() const { return data; }
  operator char*() { return &data[0]; }
  operator const char*() const { return data.c_str(); }

  TagPtr clone() const override;
  void swap(TagString& r) { data.swap(r.data); }

protected:

  bool equals(const Tag& r) const override
  {
    return *this == dynamic_cast<const TagString&>(r);
  }
};

bool operator==(const TagString& l, const TagString& r);
bool operator!=(const TagString& l, const TagString& r);

} // namespace nbt
} // namespace redi

#endif //REDI_NBT_STRING