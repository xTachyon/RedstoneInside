#include <boost/format.hpp>
#include "prettyprinter.hpp"
#include "tag.hpp"

namespace redi
{
namespace nbt
{

void PrettyPrint::writeEntry(std::size_t size)
{
  string += std::to_string(size) + (size == 1 ? " entry" : " entries");
}

void PrettyPrint::writeVectorSize(std::size_t size, const char* name)
{
  string += (boost::format("%1% %2%%3%") % size
             % name % (size == 1 ? "" : "s")).str();
}

void PrettyPrint::writeType(Type t, const std::string& name)
{
  string += getNBTTypeName(t);
  string += (boost::format("(\"%1%\")") % name).str();
  string += " : ";
}

void PrettyPrint::writeType(Type t)
{
  string += getNBTTypeName(t);
  string += " : ";
}

void PrettyPrint::writeIndent(std::int32_t i)
{
  if (i > 0)
  {
    for (; i > 0; --i)
    {
      indent += ' ';
    }
  }
  else if (i < 0)
  {
    for (; i < 0; ++i)
    {
      indent.pop_back();
    }
  }
  
  string += indent;
}

void PrettyPrint::writeNewline(std::int32_t)
{
  string += '\n';
}

void PrettyPrint::writeBeggining()
{
  writeNewline(0);
  writeIndent();
  string += '{';
}

void PrettyPrint::writeEnding()
{
  writeNewline(0);
  writeIndent();
  string += '}';
}

void PrettyPrint::writeOne(const Tag& tag)
{
  if (tag.isContainer())
  {
    writeEntry(tag.size());
  }
}
  
} // namespace nbt
} // namespace redi