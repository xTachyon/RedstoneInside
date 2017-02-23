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
  void writeType(Type t, const std::string& name);
  void writeType(Type t);
  void writeIndent(std::int32_t i = 0);
  void writeNewline(std::int32_t i = 0);
  void writeBeggining();
  void writeEnding();
  void writeOne(const Tag& tag);
  
  struct Block
  {
    PrettyPrint& pr;
    
    Block(PrettyPrint& pr) : pr(pr)
    {
      pr.writeBeggining();
    }
    
    ~Block()
    {
      pr.writeEnding();
    }
  };
  
  struct Indent
  {
    PrettyPrint& pr;
    
    Indent(PrettyPrint& pr) : pr(pr)
    {
      pr.writeIndent(2);
    }
    
    ~Indent()
    {
      pr.writeIndent(-2);
    }
  };
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_PRETTYPRINTER_HPP