#include <cassert>
#include <string>
#include <boost/format.hpp>
#include "prettyprint.hpp"
#include "scalar.hpp"
#include "string.hpp"
#include "vectorial.hpp"
#include "roottag.hpp"
#include "list.hpp"

namespace redi
{
namespace nbt
{

PrettyPrint::PrettyPrint(const RootTag& obj)
{
  generate(obj);
}

void PrettyPrint::generate(const RootTag& obj)
{
  writeRootTag(obj);
}

void PrettyPrint::writeNumeric(const Tag& obj)
{
  switch (obj.getType())
  {
    case NBTType::Byte:
      mStr += std::to_string(static_cast<short>(obj.get<NBTType::Byte>()));
      break;

    case NBTType::Short:
      mStr += std::to_string(obj.get<NBTType::Short>());
      break;

    case NBTType::Int:
      mStr += std::to_string(obj.get<NBTType::Int>());
      break;

    case NBTType::Long:
      mStr += std::to_string(obj.get<NBTType::Long>());
      break;

    case NBTType::Float:
      mStr += std::to_string(obj.get<NBTType::Float>());
      break;

    case NBTType::Double:
      mStr += std::to_string(obj.get<NBTType::Double>());
      break;

    default:
      assert(true);
  }
}

void PrettyPrint::writeVector(const Tag& obj)
{
  std::size_t elements;

  switch (obj.getType())
  {
    case NBTType::ByteArray:
      elements = obj.get<NBTType::ByteArray>().size();
      break;

    case NBTType::IntArray:
      elements = obj.get<NBTType::IntArray>().size();
      break;

    default:
      elements = 0;
      assert(true);
  }

  mStr += std::to_string(elements) + ((elements == 1) ? " element" : " elements");
}

void PrettyPrint::writeCompound(const TagCompound& obj, std::size_t indent)
{
  mStr += std::to_string(obj.size()) + ((obj.size() == 1) ? " entry" : " entries") + '\n';
  writeIndentation(indent);
  mStr += "{\n";

  for (const auto& index : obj)
  {
    Tag& t = *index.second;

    writeIndentation(indent + 2);
    mStr += (boost::format("%1%(\"%2%\") : ") % getNBTTypeName(t.getType()) % index.first).str();

    if (t.getType() == NBTType::String) mStr += (boost::format("\"%1%\"") % t.get<NBTType::String>()).str();
    else if (t.isNumeric()) writeNumeric(t);
    else if (t.isVector()) writeVector(t);
    else if (t.getType() == NBTType::Compound) writeCompound(t.get<NBTType::Compound>(), indent + 2);
    else if (t.getType() == NBTType::List) writeList(t.get<NBTType::List>(), indent + 2);

    mStr += '\n';
  }

  writeIndentation(indent);
  mStr += '}';
}

void PrettyPrint::writeIndentation(std::size_t size)
{
  for (std::size_t i = 0; i < size; ++i) mStr += ' ';
}

void PrettyPrint::writeRootTag(const RootTag& obj)
{
  mStr += (boost::format("%1%(\"%2%\") : ") % getNBTTypeName(NBTType::Compound) % obj.name).str();

  writeCompound(obj);
}

void PrettyPrint::writeList(const TagList& obj, std::size_t indent)
{
  mStr += std::to_string(obj.size()) + ((obj.size() == 1) ? " entry" : " entries") + '\n';
  writeIndentation(indent);
  mStr += "{\n";

  for (const auto& index : obj)
  {
    Tag& t = *index;

    writeIndentation(indent + 2);
    mStr += getNBTTypeName(t.getType());
    mStr += " : ";
  
    if (t.getType() == NBTType::String) mStr += (boost::format("\"%1%\"") % t.get<NBTType::String>()).str();
    else if (t.isNumeric()) writeNumeric(t);
    else if (t.isVector()) writeVector(t);
    else if (t.getType() == NBTType::Compound) writeCompound(t.get<NBTType::Compound>(), indent + 2);
    else if (t.getType() == NBTType::List) writeList(t.get<NBTType::List>(), indent + 2);

    mStr += '\n';
  }

  writeIndentation(indent);
  mStr += '}';
}

} // namespace nbt
} // namespace redi