#pragma once

#include "basic.hpp"
#include "deserializer.hpp"

namespace redi::nbt {

struct tag_string : public basic_tag<tag_string> {
  static constexpr tag_type type = tag_type::type_string;

  nbt_string data;

  tag_string() = default;
  tag_string(nbt_string_view str) : data(str) {}
  tag_string(nbt_string&& str) : data(std::move(str)) {}
//  tag_string(const char* str) : data(str) {}

  tag_string& operator=(const std::string& str) {
    data = str;
    return *this;
  }

  tag_string& operator=(std::string&& str) {
    data = std::move(str);
    return *this;
  }

  tag_string& operator=(const char* str) {
    data = str;
    return *this;
  }

//  explicit operator std::string&() { return data; }
//  explicit operator const std::string&() const { return data; }
};

} // namespace redi::nbt