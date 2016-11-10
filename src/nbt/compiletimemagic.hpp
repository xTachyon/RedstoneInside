#ifndef REDI_NBT_COMPILETIMEMAGIC
# define REDI_NBT_COMPILETIMEMAGIC

# include <string>
# include <type_traits>
# include "type.hpp"

namespace redi
{
namespace nbt
{
/**
* Helper class for getting the type of the NBT object at compile-time
* with some STL magic
*/

// TagTypeInfo is for type to enum

class TagString;
class TagCompound;
class TagList;

template <typename T>
struct TagTypeInfo {};

template <>
struct TagTypeInfo<std::int8_t> : std::integral_constant<NBTType, NBTType::Byte> {};

template <>
struct TagTypeInfo<std::int16_t> : std::integral_constant<NBTType, NBTType::Short> {};

template <>
struct TagTypeInfo<std::int32_t> : std::integral_constant<NBTType, NBTType::Int> {};

template <>
struct TagTypeInfo<std::int64_t> : std::integral_constant<NBTType, NBTType::Long> {};

template <>
struct TagTypeInfo<float> : std::integral_constant<NBTType, NBTType::Float> {};

template <>
struct TagTypeInfo<double> : std::integral_constant<NBTType, NBTType::Double> {};

template <>
struct TagTypeInfo<std::string> : std::integral_constant<NBTType, NBTType::String> {};

template <>
struct TagTypeInfo<TagCompound> : std::integral_constant<NBTType, NBTType::Compound> {};

// Forward declaration

template <typename T>
class ScalarTag;

using TagByte = ScalarTag<std::int8_t>;
using TagShort = ScalarTag<std::int16_t>;
using TagInt = ScalarTag<std::int32_t>;
using TagLong = ScalarTag<std::int64_t>;
using TagFloat = ScalarTag<float>;
using TagDouble = ScalarTag<double>;

// and TagTypeInfoType is for enum to type

template <NBTType type>
struct TagTypeInfoType {};

template <>
struct TagTypeInfoType<NBTType::Byte>
{
  using returnType = std::int8_t;
  using castType = TagByte;
};

template <>
struct TagTypeInfoType<NBTType::Short>
{
  using returnType = std::int16_t;
  using castType = TagShort;
};

template <>
struct TagTypeInfoType<NBTType::Int>
{
  using returnType = std::int32_t;
  using castType = TagInt;
};

template <>
struct TagTypeInfoType<NBTType::Long>
{
  using returnType = std::int64_t;
  using castType = TagLong;
};

template <>
struct TagTypeInfoType<NBTType::Float>
{
  using returnType = float;
  using castType = TagFloat;
};

template <>
struct TagTypeInfoType<NBTType::Double>
{
  using returnType = double;
  using castType = TagDouble;
};

template <>
struct TagTypeInfoType<NBTType::String>
{
  using returnType = std::string;
  using castType = TagString;
};

template <>
struct TagTypeInfoType<NBTType::List>
{
  using returnType = TagList;
  using castType = TagList;
};

template <>
struct TagTypeInfoType<NBTType::Compound>
{
  using returnType = TagCompound;
  using castType = TagCompound;
};

// Best names, I know

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_COMPILETIMEMAGIC