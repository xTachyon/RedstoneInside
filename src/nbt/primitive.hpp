#pragma once

#include "basic.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "prettyprinter.hpp"

namespace redi::nbt {

template <typename T>
struct primitive : public basic_tag<primitive<T>> {
  static constexpr tag_type type = type_to_number<T>::value;

  T data;

  explicit primitive(T value = 0) : data(value) {}

  primitive& operator=(T value) {
    data = value;
    return *this;
  }

//  explicit operator T&() { return data; }
//  explicit operator T() const { return data; }
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, const primitive<T>& obj) {
  stream << obj.data;
  return stream;
}

using tag_byte = primitive<nbt_byte>;
using tag_short = primitive<nbt_short>;
using tag_int = primitive<nbt_int>;
using tag_long = primitive<nbt_long>;
using tag_float = primitive<nbt_float>;
using tag_double = primitive<nbt_double>;

} // namespace redi::nbt