#include "primitive.hpp"
#include "string.hpp"
#include "compound.hpp"
#include "value.hpp"
#include "basic.hpp"
#include "primitive.hpp"
#include "string.hpp"
#include "compound.hpp"
#include "list.hpp"
#include "array.hpp"

namespace redi
{
namespace nbt
{

Value::Value(const Value& obj) : data(obj.data ? obj.data->clone() : nullptr) {}

Value::Value(Tag&& tag) : data(std::move(tag).move()) {}

Value::Value(std::int8_t x) : Value(TagByte(x)) {}

Value::Value(std::int16_t x) : Value(TagShort(x)) {}

Value::Value(std::int32_t x) : Value(TagInt(x)) {}

Value::Value(std::int64_t x) : Value(TagLong(x)) {}

Value::Value(float x) : Value(TagFloat(x)) {}

Value::Value(double x) : Value(TagDouble(x)) {}

Value::Value(const std::string& x) : Value(TagString(x)) {}

Value::Value(std::string&& x) : Value(TagString(std::move(x))) {}

Value::Value(const char* x) : Value(TagString(x)) {}

Value::Value(TagUniquePtr&& ptr) : data(std::move(ptr)) {}

Value::Value(std::nullptr_t ) : Value(){}

Value& Value::operator=(const Value& other)
{
  if (data)
  {
    data->assign(*other.data);
  }
  else
  {
    data = other.data->clone();
  }
  
  return *this;
}

Value& Value::operator=(const Tag& tag)
{
  *this = tag.clone();
  
  return *this;
}

Value& Value::operator=(Tag&& tag)
{
  if (data)
  {
    data->assign(std::move(tag));
  }
  else
  {
    data = std::move(tag).move();
  }
  
  return *this;
}

Value& Value::operator=(TagUniquePtr&& ptr)
{
  if (ptr)
  {
    *this = std::move(*ptr);
  }
  else if (data)
  {
    throw std::bad_cast();
  }
  
  return *this;
}

Value& Value::operator=(std::int8_t x)
{
  *this = TagByte(x);
  return *this;
}

Value& Value::operator=(std::int16_t x)
{
  *this = TagShort(x);
  return *this;
}

Value& Value::operator=(std::int32_t x)
{
  *this = TagInt(x);
  return *this;
}

Value& Value::operator=(std::int64_t x)
{
  *this = TagLong(x);
  return *this;
}

Value& Value::operator=(float x)
{
  *this = TagFloat(x);
  return *this;
}

Value& Value::operator=(double x)
{
  *this = TagDouble(x);
  return *this;
}

Value& Value::operator=(const std::string& x)
{
  *this = TagString(x);
  return *this;
}

Value& Value::operator=(std::string&& x)
{
  *this = TagString(std::move(x));
  return *this;
}

Value& Value::operator=(std::nullptr_t)
{
  throwIfNot(Type::End);
  return *this;
}

Value& Value::operator[](const std::string& key)
{
  throwIfNot(Type::Compound);
  return static_cast<TagCompound&>(*data)[key];
}

Value& Value::operator[](std::string&& key)
{
  throwIfNot(Type::Compound);
  return static_cast<TagCompound&>(*data)[std::move(key)];
}

Value& Value::operator[](const char* key)
{
  return (*this)[std::string(key)];
}

void Value::assign(Tag&& tag)
{
  *this = std::move(tag);
}

Type Value::getType() const
{
  return getType(data);
}

Type Value::getType(const TagUniquePtr& ptr)
{
  return ptr ? ptr->getType() : Type::End;
}

void Value::write(Serializer& s) const
{
  data->write(s);
}

void Value::writePretty(PrettyPrint& p) const
{
  if (data)
  {
    data->writePretty(p);
  }
  else
  {
    p.string += "null";
  }
}

void Value::throwIfNot(Type type) const
{
  if (getType() != type)
  {
    throw std::bad_cast();
  }
}

void Value::throwIfNullOrIsNot(Type type) const
{
  if (!data)
  {
    throw std::runtime_error("Tag is null");
  }
  
  throwIfNot(type);
}

std::int8_t& Value::getByte()
{
  throwIfNot(Type::Byte);
  return static_cast<TagByte&>(*data).data;
}

std::int16_t& Value::getShort()
{
  throwIfNot(Type::Short);
  return static_cast<TagShort&>(*data).data;
}

std::int32_t& Value::getInt()
{
  throwIfNot(Type::Int);
  return static_cast<TagInt&>(*data).data;
}

std::int64_t& Value::getLong()
{
  throwIfNot(Type::Long);
  return static_cast<TagLong&>(*data).data;
}

float& Value::getFloat()
{
  throwIfNot(Type::Float);
  return static_cast<TagFloat&>(*data).data;
}

double& Value::getDouble()
{
  throwIfNot(Type::Double);
  return static_cast<TagDouble&>(*data).data;
}

std::vector<std::int8_t>& Value::getByteArray()
{
  throwIfNot(Type::ByteArray);
  return static_cast<TagByteArray&>(*data).data;
}

std::string& Value::getString()
{
  throwIfNot(Type::String);
  return static_cast<TagString&>(*data).data;
}

TagList& Value::getList()
{
  throwIfNot(Type::List);
  return static_cast<TagList&>(*data);
}

TagCompound& Value::getCompound()
{
  throwIfNot(Type::Compound);
  return static_cast<TagCompound&>(*data);
}

std::vector<std::int32_t>& Value::getIntArray()
{
  throwIfNot(Type::IntArray);
  return static_cast<TagIntArray&>(*data).data;
}

std::vector<std::int16_t>& Value::getShortArray()
{
  throwIfNot(Type::ShortArray);
  return static_cast<TagShortArray&>(*data).data;
}

const std::int8_t& Value::getByte() const
{
  throwIfNot(Type::Byte);
  return static_cast<const TagByte&>(*data).data;
}

const std::int16_t& Value::getShort() const
{
  throwIfNot(Type::Short);
  return static_cast<const TagShort&>(*data).data;
}

const std::int32_t& Value::getInt() const
{
  throwIfNot(Type::Int);
  return static_cast<const TagInt&>(*data).data;
}

const std::int64_t& Value::getLong() const
{
  throwIfNot(Type::Long);
  return static_cast<const TagLong&>(*data).data;
}

const float& Value::getFloat() const
{
  throwIfNot(Type::Float);
  return static_cast<const TagFloat&>(*data).data;
}

const double& Value::getDouble() const
{
  throwIfNot(Type::Double);
  return static_cast<const TagDouble&>(*data).data;
}

const std::vector<std::int8_t>& Value::getByteArray() const
{
  throwIfNot(Type::ByteArray);
  return static_cast<const TagByteArray&>(*data).data;
}

const std::string& Value::getString() const
{
  throwIfNot(Type::String);
  return static_cast<const TagString&>(*data).data;
}

const TagList& Value::getList() const
{
  throwIfNot(Type::List);
  return static_cast<const TagList&>(*data);
}

const TagCompound& Value::getCompound() const
{
  throwIfNot(Type::Compound);
  return static_cast<const TagCompound&>(*data);
}

const std::vector<std::int32_t>& Value::getIntArray() const
{
  throwIfNot(Type::IntArray);
  return static_cast<const TagIntArray&>(*data).data;
}

const std::vector<std::int16_t>& Value::getShortArray() const
{
  throwIfNot(Type::ShortArray);
  return static_cast<const TagShortArray&>(*data).data;
}
  
} // namespace nbt
} // namespace redi