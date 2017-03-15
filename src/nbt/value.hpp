#ifndef REDI_NBT_VALUE_HPP
#define REDI_NBT_VALUE_HPP

#include <memory>
#include <vector>
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
  Value(TagUniquePtr&& ptr);
  Value(std::nullptr_t x);
  
  Value& operator=(const Value& other);
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
  
  operator bool() const { return static_cast<bool>(data); }
  
  const Tag& get() const { return *data; }
  Tag& get() { return *data; }
  
  void assign(Tag&& tag);
  Type getType() const;
  static Type getType(const TagUniquePtr& ptr);
  void write(Serializer& s) const;
  void writePretty(PrettyPrint& p) const;
  
  std::int8_t& getByte();
  std::int16_t& getShort();
  std::int32_t& getInt();
  std::int64_t& getLong();
  float& getFloat();
  double& getDouble();
  std::vector<std::int8_t>& getByteArray();
  std::string& getString();
  TagList& getList();
  TagCompound& getCompound();
  std::vector<std::int32_t>& getIntArray();
  std::vector<std::int16_t>& getShortArray();
  
  const std::int8_t& getByte() const;
  const std::int16_t& getShort() const;
  const std::int32_t& getInt() const;
  const std::int64_t& getLong() const;
  const float& getFloat() const;
  const double& getDouble() const;
  const std::vector<std::int8_t>& getByteArray() const;
  const std::string& getString() const;
  const TagList& getList() const;
  const TagCompound& getCompound() const;
  const std::vector<std::int32_t>& getIntArray() const;
  const std::vector<std::int16_t>& getShortArray() const;
  
private:
  
  std::unique_ptr<Tag> data;
  
  void throwIfNot(Type type) const;
  void throwIfNullOrIsNot(Type type) const;
};
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_VALUE_HPP