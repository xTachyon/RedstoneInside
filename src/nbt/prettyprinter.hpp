#ifndef REDI_NBT_PRETTYPRINTER_HPP
#define REDI_NBT_PRETTYPRINTER_HPP

#include <string>
#include "type.hpp"

namespace redi
{
namespace nbt
{

struct PrettyPrint
{
  std::string string;
  std::string indent;
  
  void writeEntry(std::size_t size);
  void writeVectorSize(std::size_t size, const char* name);
  void writeType(Type t, const std::string& name = "");
  void writeIndent();
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_PRETTYPRINTER_HPP