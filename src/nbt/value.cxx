#include "primitive.hpp"
#include "string.hpp"
#include "compound.hpp"
#include "value.hpp"

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

Value::Value(std::nullptr_t ) : Value(){}

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

void Value::throwIfNot(Type type)
{
  if (getType() != type)
  {
    throw std::bad_cast();
  }
}
  
} // namespace nbt
} // namespace redi