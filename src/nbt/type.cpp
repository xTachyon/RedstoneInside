#include "type.hpp"

namespace redi::nbt {

const char* getNBTTypeName(tag_type type) {
  const char* names[] = {"TAG_End",    "TAG_Byte",       "TAG_Short",
                         "TAG_Int",    "TAG_Long",       "TAG_Float",
                         "TAG_Double", "TAG_Byte_Array", "TAG_String",
                         "TAG_List",   "TAG_Compound",   "TAG_Int_Array",
                         "TAG_Long_Array"};

  return names[static_cast<std::size_t>(type)];
}

std::ostream& operator<<(std::ostream& stream, tag_type type) {
  stream << getNBTTypeName(type);
  return stream;
}

} // namespace redi::nbt