#include "value.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "creator.hpp"
#include "prettyprinter.hpp"
#include "list.hpp"

namespace redi
{
namespace nbt
{

TagList::TagList(const TagList& other)
{
  *this = other;
}

TagList& TagList::operator=(const TagList& other)
{
  data = other.data;
  
  return *this;
}

void TagList::write(Serializer& s) const
{
  Type t = getListType();
  s.writeType(t);
  s.writeNumber<std::int32_t>(static_cast<std::int32_t>(size()));
  
  for (const auto& index : data)
  {
    index.write(s);
  }
}

Type TagList::getListType() const
{
  return empty() ? Type::End : data.front().getType();
}

void TagList::read(Deserializer& s)
{
  Type t = s.readType();
  std::size_t size = static_cast<std::size_t>(s.readNumber<std::int32_t>());
  data.reserve(size);
  
  for (std::size_t i = 0; i < size; ++i)
  {
    TagUniquePtr tag = create(t);
    if (t != Type::End)
    {
      tag->read(s);
    }
    data.emplace_back(std::move(tag));
  }
}

void TagList::writePretty(PrettyPrint& p) const
{
  PrettyPrint::Block b(p);
  PrettyPrint::Indent i(p);
  Type t = getListType();
  
  for (const auto& index : data)
  {
    p.writeNewline();
    p.writeIndent();
    
    p.writeType(t);
    if (index)
    {
      p.writeOne(index.get());
    }
    
    index.writePretty(p);
  }
}
  
} // namespace nbt
} // namespace redi