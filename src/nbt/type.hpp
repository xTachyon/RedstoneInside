#pragma once

#include <ostream>
#include <type_traits>
#include "forward.hpp"

namespace redi::nbt {

enum class tag_type : std::uint8_t {
  type_end,
  type_byte,
  type_short,
  type_int,
  type_long,
  type_float,
  type_double,
  type_byte_array,
  type_string,
  type_list,
  type_compound,
  type_int_array,
  type_long_array
};

const char* getNBTTypeName(tag_type type);
std::ostream& operator<<(std::ostream& stream, tag_type type);

template <typename T>
struct type_to_number {};

template <>
struct type_to_number<nbt_byte>
    : public std::integral_constant<tag_type, tag_type::type_byte> {};

template <>
struct type_to_number<nbt_short>
    : public std::integral_constant<tag_type, tag_type::type_short> {};

template <>
struct type_to_number<nbt_int>
    : public std::integral_constant<tag_type, tag_type::type_int> {};

template <>
struct type_to_number<nbt_long>
    : public std::integral_constant<tag_type, tag_type::type_long> {};

template <>
struct type_to_number<nbt_float> : public std::integral_constant<tag_type, tag_type::type_float> {
};

template <>
struct type_to_number<nbt_double>
    : public std::integral_constant<tag_type, tag_type::type_double> {};

template <>
struct type_to_number<array<nbt_byte>>
    : public std::integral_constant<tag_type, tag_type::type_byte_array> {};

template <>
struct type_to_number<array<nbt_int>>
    : public std::integral_constant<tag_type, tag_type::type_int_array> {};

template <>
struct type_to_number<array<nbt_long>>
      : public std::integral_constant<tag_type, tag_type::type_long_array> {};

} // namespace redi::nbt