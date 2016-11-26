#include <cassert>
#include <string>
#include <boost/format.hpp>
#include "printer.hpp"
#include "scalar.hpp"
#include "string.hpp"
#include "vectorial.hpp"
#include "roottag.hpp"

namespace redi
{
namespace nbt
{

Printer::Printer(const RootTag& obj)
{
  generate(obj);
}

//Printer& Printer::operator=(const RootTag& obj)
//{
//
//
//  return *this;
//}

void Printer::generate(const RootTag& obj)
{
  writeRootTag(obj);
}

void Printer::writeScalar(const Tag& obj)
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

    case NBTType::String:
      mStr += obj.get<NBTType::String>();
      break;

    default:
      assert(true);
  }
}

void Printer::writeVector(const Tag& obj)
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
      assert(true);
  }

  mStr += std::to_string(elements) + ((elements == 1) ? " element" : " elements");
}

void Printer::writeCompound(const TagCompound& obj, std::size_t indent)
{
  mStr += std::to_string(obj.size()) + ((obj.size() == 1) ? " entry" : " entries") + '\n';
  writeIndentation(indent);
  mStr += "{\n";

  for (const auto& index : obj)
  {
    Tag& t = *index.second;

    writeIndentation(indent + 2);
    mStr += (boost::format("%1%(\"%2%\") : ") % getNBTTypeName(t.getType()) % index.first).str();

    if (t.isScalar()) writeScalar(t);
    else if (t.isVector()) writeVector(t);
    else if (t.getType() == NBTType::Compound) writeCompound(t.get<NBTType::Compound>(), indent + 2);

    mStr += '\n';
  }

  writeIndentation(indent);
  mStr += '}';
}

void Printer::writeIndentation(std::size_t size)
{
  for (std::size_t i = 0; i < size; ++i) mStr += ' ';
}

void Printer::writeRootTag(const RootTag& obj)
{
  // mStr += (boost::format("%1%(\"%2%\") : %3% %4%\n") % getNBTTypeName(NBTType::Compound) % obj.name % obj.size() % ((obj.size() == 1) ? "entry" : "entries")).str();
  mStr += (boost::format("%1%(\"%2%\") : ") % getNBTTypeName(NBTType::Compound) % obj.name).str();


  writeCompound(obj);
}

} // namespace nbt
} // namespace redi