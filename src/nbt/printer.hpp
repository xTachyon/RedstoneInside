#ifndef REDI_NBT_PRINTER
#define REDI_NBT_PRINTER

#include <string>
#include "tag.hpp"
#include "roottag.hpp"

namespace redi
{
namespace nbt
{

class Printer
{
public:

  Printer(const RootTag& obj);

  //Printer& operator=(const RootTag& obj);

  operator const std::string&() const { return mStr; }
  operator const char*() const { return mStr.c_str(); }

  const std::string& str() const { return mStr; }

private:

  std::string mStr;

  void generate(const RootTag& obj);
  void writeScalar(const Tag& obj);
  void writeVector(const Tag& obj);
  void writeCompound(const TagCompound& obj, std::size_t indent = 0);
  void writeIndentation(std::size_t size = 0);
  void writeRootTag(const RootTag& obj);
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_PRINTER
