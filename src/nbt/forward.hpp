#pragma once

#include <cstdint>
#include "datatypes.h"

namespace redi::nbt {

class tag;

template <typename T>
class basic_tag;

template <typename T>
struct primitive;

template <typename T>
struct array;


using tag_byte = primitive<nbt_byte>;
using tag_short = primitive<nbt_short>;
using tag_int = primitive<nbt_int>;
using tag_long = primitive<nbt_long>;
using tag_float = primitive<nbt_float>;
using tag_double = primitive<nbt_double>;

using tag_byte_array = array<nbt_byte>;
using tag_int_array = array<nbt_int>;
using tag_long_array = array<nbt_long>;

class tag_end;
struct tag_string;
class tag_compound;
class tag_list;
struct root_tag;

struct Deserializer;
struct pretty_printer;
struct serializer;
class tag_value;

class nbt_visitor;
class const_nbt_visitor;

} // namespace redi::nbt