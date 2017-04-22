#ifndef REDI_DATATYPES_HPP
#define REDI_DATATYPES_HPP

#include <cstdint>
#include <string_view>

namespace redi {
using byte = unsigned char;
using sbyte = std::int8_t;

template <typename T>
using basic_string_view = std::basic_string_view<T>;
using string_view = std::string_view;

}

#endif // REDI_DATATYPES_HPP