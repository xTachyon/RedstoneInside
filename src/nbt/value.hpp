#pragma once

#include <memory>
#include <vector>
#include "tag.hpp"
#include "prettyprinter.hpp"
#include "array.hpp"

namespace redi::nbt {

class tag_value {
public:
  tag_value() = default;
  tag_value(const tag_value& obj);
  tag_value(tag_value&& obj) = default;
  explicit tag_value(tag&& tag);
  explicit tag_value(nbt_byte x);
  explicit tag_value(nbt_short x);
  explicit tag_value(nbt_int x);
  explicit tag_value(nbt_long x);
  explicit tag_value(nbt_float x);
  explicit tag_value(nbt_double x);
  explicit tag_value(nbt_string_view x);
  explicit tag_value(nbt_string&& x);
  explicit tag_value(tag_unique_ptr&& ptr);
  explicit tag_value(std::nullptr_t x);

  tag_value& operator=(const tag_value& other);
  tag_value& operator=(const tag& tag);
  tag_value& operator=(tag&& tag);
  tag_value& operator=(tag_unique_ptr&& ptr);

  tag_value& operator=(std::int8_t x);
  tag_value& operator=(std::int16_t x);
  tag_value& operator=(std::int32_t x);
  tag_value& operator=(std::int64_t x);
  tag_value& operator=(float x);
  tag_value& operator=(double x);
  tag_value& operator=(const std::string& x);
  tag_value& operator=(std::string&& x);
  tag_value& operator=(std::nullptr_t x);

  tag_value& operator=(tag_value&& other) noexcept;

  tag_value& operator[](const nbt_string& key);
  tag_value& operator[](nbt_string&& key);
  tag_value& operator[](const char* key);

  explicit operator bool() const { return static_cast<bool>(data); }

  const tag& get() const { return *data; }
  tag& get() { return *data; }

//  void assign(tag&& tag);
  tag_type get_type() const;
  static tag_type get_type(const tag_unique_ptr& ptr);

  nbt_byte& getByte();
  nbt_short& getShort();
  nbt_int& getInt();
  nbt_long& getLong();
  nbt_float& getFloat();
  nbt_double& getDouble();
  tag_byte_array_container& getByteArray();
  nbt_string& getString();
  tag_list& getList();
  tag_compound& getCompound();
  tag_int_array_container& getIntArray();
  tag_long_array_container& getLongArray();

  const nbt_byte& getByte() const;
  const nbt_short& getShort() const;
  const nbt_int& getInt() const;
  const nbt_long& getLong() const;
  const nbt_float& getFloat() const;
  const nbt_double& getDouble() const;
  const tag_byte_array_container& getByteArray() const;
  const nbt_string& getString() const;
  const tag_list& getList() const;
  const tag_compound& getCompound() const;
  const tag_int_array_container& getIntArray() const;
  const tag_long_array_container& getLongArray() const;

  void visit(nbt_visitor&);
  void visit(const_nbt_visitor&) const;

private:
  std::unique_ptr<tag> data;

  void throwIfNot(tag_type type) const;
  void throwIfNullOrIsNot(tag_type type) const;
};

} // namespace redi::nbt