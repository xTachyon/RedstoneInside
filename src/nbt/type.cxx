#include "type.hpp"

namespace redi
{
namespace nbt
{
const char* getNBTTypeName(Type type)
{
  const char* names[] = {
        "TAG_End",
        "TAG_Byte",
        "TAG_Short",
        "TAG_Int",
        "TAG_Long",
        "TAG_Float",
        "TAG_Double",
        "TAG_Byte_Array",
        "TAG_String",
        "TAG_List",
        "TAG_Compound",
        "TAG_Int_Array"
  };
  
  return names[static_cast<std::size_t>(type)];
}

std::ostream& operator<<(std::ostream& stream, Type type)
{
  stream << getNBTTypeName(type);
  return stream;
}
} // namespace nbt
} // namespace redi