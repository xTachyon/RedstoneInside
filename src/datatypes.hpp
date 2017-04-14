#ifndef REDI_DATATYPES_HPP
#define REDI_DATATYPES_HPP

#include <cstdint>
#include <experimental/string_view>

namespace redi {
using byte = std::uint8_t;
using sbyte = std::int8_t;

template <typename T>
using basic_string_view = std::experimental::basic_string_view<T>;
using string_view = std::experimental::string_view;
}

#endif // REDI_DATATYPES_HPP