#ifndef REDI_NBT_STRING_HPP
#define REDI_NBT_STRING_HPP

#include "basic.hpp"

namespace redi
{
namespace nbt
{

struct TagString : public BasicTag<TagString>
{
  static constexpr Type type = Type::String;
  
  std::string data;
  
  TagString() {}
  TagString(const std::string& str) : data(str) {}
  TagString(std::string&& str) : data(std::move(str)) {}
  TagString(const char* str) : data(str) {}
  
  TagString& operator=(const std::string& str)
  {
    data = str;
    return *this;
  }
  
  TagString& operator=(std::string&& str)
  {
    data = std::move(str);
    return *this;
  }
  
  TagString& operator=(const char* str)
  {
    
    data = str;
    return *this;
  }
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_STRING_HPP