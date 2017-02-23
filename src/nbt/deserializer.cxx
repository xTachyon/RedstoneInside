#include "compound.hpp"
#include "roottag.hpp"
#include "deserializer.hpp"

namespace redi
{
namespace nbt
{

Deserializer::Deserializer(const ByteBuffer& buffer, size_t offset)
  : buffer(buffer), offset(offset) {}

void Deserializer::operator()(std::string& name, TagCompound& root)
{
  read(name, root);
}

void Deserializer::operator()(TagCompound& root)
{
  read(root);
}

void Deserializer::need(std::size_t bytes)
{
  if (bytes + offset > buffer.size())
  {
    throw std::runtime_error(std::to_string(bytes) + " is out of index");
  }
}

std::string Deserializer::readString()
{
  std::size_t size = readNumber<std::uint16_t>();
  need(size);
  std::string s(buffer.as_const_char() + offset, buffer.as_const_char() + offset + size);
  offset += size;
  return s;
}

Type Deserializer::readType()
{
  need(1);
  std::uint8_t x = buffer[offset++];
  if (x > 12)
  {
    throw std::runtime_error(std::to_string(static_cast<int>(x)) + " is an unrecognized NBT tag");
  }
  return static_cast<Type>(x);
}

void Deserializer::read(std::string& name, TagCompound& root)
{
  Type t = readType();
  if (t != Type::Compound)
  {
    throw std::runtime_error("Root tag must be compound");
  }
  name = readString();
  root.read(*this);
}

void Deserializer::read(TagCompound& root)
{
  std::string name;
  read(name, root);
}

void Deserializer::read(RootTag& root)
{
  read(root.name, root);
}
  
} // namespace nbt
} // namespace redi