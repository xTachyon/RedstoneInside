#include "value.hpp"
#include "compound.hpp"
#include "serializer.hpp"
#include "roottag.hpp"

namespace redi
{
namespace nbt
{

Serializer::Serializer(ByteBuffer& buffer) : buffer(buffer) {}

ByteBuffer& Serializer::operator()(const std::string& name, const TagCompound& root)
{
  write(name, root);
  return buffer;
}

void Serializer::writeString(const std::string& str)
{
  writeNumber(static_cast<std::uint16_t>(str.size()));
  buffer.append(str.c_str(), str.size());
}

void Serializer::write(const std::string& name, const TagCompound& root)
{
  writeType(Type::Compound);
  writeString(name);
  root.write(*this);
}

void Serializer::writeType(const Value& value)
{
  writeType(value.getType());
}

void Serializer::write(const RootTag& root)
{
  write(root.name, root);
}

void Serializer::writeType(Type type)
{
  writeNumber(static_cast<std::uint8_t>(type));
}

} // namespace nbt
} // namespace redi