#include <boost/format.hpp>
#include "prettyprinter.hpp"

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
  if (name.size() > 0)
  {
    string += (boost::format("(\"%1%\")") % name).str();
  }
  string += ": ";
}
  
} // namespace nbt
} // namespace redi