#ifndef REDI_NBT_VALUE_HPP
#define REDI_NBT_VALUE_HPP

#include <memory>
#include "tag.hpp"

namespace redi
{
namespace nbt
{

class Value
{
public:
  
  Value() = default;
  Value(const Value& obj);
  Value(Value&& obj) = default;
  Value(Tag&& tag);
  
  Value(std::int8_t x);
  Value(std::int16_t x);
  Value(std::int32_t x);
  Value(std::int64_t x);
  Value(float x);
  Value(double x);
  Value(const std::string& x);
  Value(std::string&& x);
  Value(const char* x);
  Value(std::nullptr_t x);
  
  Value& operator=(const Tag& tag);
  Value& operator=(Tag&& tag);
  Value& operator=(TagUniquePtr&& ptr);
  
  Value& operator=(std::int8_t x);
  Value& operator=(std::int16_t x);
  Value& operator=(std::int32_t x);
  Value& operator=(std::int64_t x);
  Value& operator=(float x);
  Value& operator=(double x);
  Value& operator=(const std::string& x);
  Value& operator=(std::string&& x);
  Value& operator=(std::nullptr_t x);
  
  Value& operator[](const std::string& key);
  Value& operator[](std::string&& key);
  Value& operator[](const char* key);
  
  void assign(Tag&& tag);
  Type getType() const;
  static Type getType(const TagUniquePtr& ptr);
  
private:
  
  std::unique_ptr<Tag> data;
  
  void throwIfNot(Type type);
};
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_VALUE_HPP