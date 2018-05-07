#pragma once

#include <cstdint>
#include <string_view>

namespace redi::nbt {

using nbt_byte = std::int8_t;
using nbt_short = std::int16_t;
using nbt_int = std::int32_t;
using nbt_long = std::int64_t;
using nbt_float = float;
using nbt_double = double;

using nbt_string = std::string;
using nbt_string_view = std::string_view;

}