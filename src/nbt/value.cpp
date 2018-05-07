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

namespace redi::nbt {

tag_value::tag_value(const tag_value& obj) : data(obj.data ? obj.data->clone() : nullptr) {}
tag_value::tag_value(tag&& tag) : data(std::move(tag).move()) {}

tag_value::tag_value(std::int8_t x) : tag_value(tag_byte(x)) {}
tag_value::tag_value(std::int16_t x) : tag_value(tag_short(x)) {}
tag_value::tag_value(std::int32_t x) : tag_value(tag_int(x)) {}
tag_value::tag_value(std::int64_t x) : tag_value(tag_long(x)) {}
tag_value::tag_value(float x) : tag_value(tag_float(x)) {}
tag_value::tag_value(double x) : tag_value(tag_double(x)) {}
tag_value::tag_value(nbt_string_view x) : tag_value(tag_string(x)) {}
tag_value::tag_value(std::string&& x) : tag_value(tag_string(std::move(x))) {}
tag_value::tag_value(tag_unique_ptr&& ptr) : data(std::move(ptr)) {}
tag_value::tag_value(std::nullptr_t) : tag_value() {}

tag_value& tag_value::operator=(const tag_value& other) {
  if (data) {
    data->assign(*other.data);
  } else {
    data = other.data->clone();
  }

  return *this;
}

tag_value& tag_value::operator=(const tag& tag) {
  *this = tag.clone();

  return *this;
}

tag_value& tag_value::operator=(tag&& tag) {
  if (data) {
    data->assign(std::move(tag));
  } else {
    data = std::move(tag).move();
  }

  return *this;
}

tag_value& tag_value::operator=(tag_unique_ptr&& ptr) {
  if (ptr) {
    *this = std::move(*ptr);
  } else if (data) {
    throw std::bad_cast();
  }

  return *this;
}

tag_value& tag_value::operator=(std::int8_t x) {
  *this = tag_byte(x);
  return *this;
}

tag_value& tag_value::operator=(std::int16_t x) {
  *this = tag_short(x);
  return *this;
}

tag_value& tag_value::operator=(std::int32_t x) {
  *this = tag_int(x);
  return *this;
}

tag_value& tag_value::operator=(std::int64_t x) {
  *this = tag_long(x);
  return *this;
}

tag_value& tag_value::operator=(float x) {
  *this = tag_float(x);
  return *this;
}

tag_value& tag_value::operator=(double x) {
  *this = tag_double(x);
  return *this;
}

tag_value& tag_value::operator=(const std::string& x) {
  *this = tag_string(x);
  return *this;
}

tag_value& tag_value::operator=(std::string&& x) {
  *this = tag_string(std::move(x));
  return *this;
}

tag_value& tag_value::operator=(std::nullptr_t) {
  throwIfNot(tag_type::type_end);
  return *this;
}

tag_value& tag_value::operator[](const std::string& key) {
  throwIfNot(tag_type::type_compound);
  return static_cast<tag_compound&>(*data)[key];
}

tag_value& tag_value::operator[](std::string&& key) {
  throwIfNot(tag_type::type_compound);
  return static_cast<tag_compound&>(*data)[std::move(key)];
}

tag_value& tag_value::operator[](const char* key) { return (*this)[std::string(key)]; }

//void tag_value::assign(tag&& tag) { *this = std::move(tag); }

tag_type tag_value::get_type() const { return get_type(data); }

tag_type tag_value::get_type(const tag_unique_ptr& ptr) {
  return ptr ? ptr->get_type() : tag_type::type_end;
}

void tag_value::throwIfNot(tag_type type) const {
  if (get_type() != type) {
    throw std::bad_cast();
  }
}

void tag_value::throwIfNullOrIsNot(tag_type type) const {
  if (!data) {
    throw std::runtime_error("Tag is null");
  }

  throwIfNot(type);
}

nbt_byte& tag_value::getByte() {
  throwIfNot(tag_type::type_byte);
  return static_cast<tag_byte&>(*data).data;
}

nbt_short& tag_value::getShort() {
  throwIfNot(tag_type::type_short);
  return static_cast<tag_short&>(*data).data;
}

nbt_int& tag_value::getInt() {
  throwIfNot(tag_type::type_int);
  return static_cast<tag_int&>(*data).data;
}

nbt_long& tag_value::getLong() {
  throwIfNot(tag_type::type_long);
  return static_cast<tag_long&>(*data).data;
}

nbt_float& tag_value::getFloat() {
  throwIfNot(tag_type::type_float);
  return static_cast<tag_float&>(*data).data;
}

nbt_double& tag_value::getDouble() {
  throwIfNot(tag_type::type_double);
  return static_cast<tag_double&>(*data).data;
}

tag_byte_array_container& tag_value::getByteArray() {
  throwIfNot(tag_type::type_byte_array);
  return static_cast<tag_byte_array&>(*data).data;
}

nbt_string& tag_value::getString() {
  throwIfNot(tag_type::type_string);
  return static_cast<tag_string&>(*data).data;
}

tag_list& tag_value::getList() {
  throwIfNot(tag_type::type_list);
  return static_cast<tag_list&>(*data);
}

tag_compound& tag_value::getCompound() {
  throwIfNot(tag_type::type_compound);
  return static_cast<tag_compound&>(*data);
}

tag_int_array_container& tag_value::getIntArray() {
  throwIfNot(tag_type::type_int_array);
  return static_cast<tag_int_array&>(*data).data;
}

tag_long_array_container& tag_value::getLongArray() {
  throwIfNot(tag_type::type_long_array);
  return static_cast<tag_long_array&>(*data).data;
}

const nbt_byte& tag_value::getByte() const {
  throwIfNot(tag_type::type_byte);
  return static_cast<const tag_byte&>(*data).data;
}

const nbt_short& tag_value::getShort() const {
  throwIfNot(tag_type::type_short);
  return static_cast<const tag_short&>(*data).data;
}

const nbt_int& tag_value::getInt() const {
  throwIfNot(tag_type::type_int);
  return static_cast<const tag_int&>(*data).data;
}

const nbt_long& tag_value::getLong() const {
  throwIfNot(tag_type::type_long);
  return static_cast<const tag_long&>(*data).data;
}

const nbt_float& tag_value::getFloat() const {
  throwIfNot(tag_type::type_float);
  return static_cast<const tag_float&>(*data).data;
}

const nbt_double& tag_value::getDouble() const {
  throwIfNot(tag_type::type_double);
  return static_cast<const tag_double&>(*data).data;
}

const tag_byte_array_container& tag_value::getByteArray() const {
  throwIfNot(tag_type::type_byte_array);
  return static_cast<const tag_byte_array&>(*data).data;
}

const nbt_string& tag_value::getString() const {
  throwIfNot(tag_type::type_string);
  return static_cast<const tag_string&>(*data).data;
}

const tag_list& tag_value::getList() const {
  throwIfNot(tag_type::type_list);
  return static_cast<const tag_list&>(*data);
}

const tag_compound& tag_value::getCompound() const {
  throwIfNot(tag_type::type_compound);
  return static_cast<const tag_compound&>(*data);
}

const tag_int_array_container& tag_value::getIntArray() const {
  throwIfNot(tag_type::type_int_array);
  return static_cast<const tag_int_array&>(*data).data;
}

const tag_long_array_container& tag_value::getLongArray() const {
  throwIfNot(tag_type::type_long_array);
  return static_cast<const tag_long_array&>(*data).data;
}

void tag_value::visit(nbt_visitor& visitor) {
  data->visit(visitor);
}

void tag_value::visit(const_nbt_visitor& visitor) const {
  data->visit(visitor);
}

tag_value& tag_value::operator=(tag_value&& other) noexcept {
  data = std::move(other.data);
  return *this;
}

} // namespace redi::nbt