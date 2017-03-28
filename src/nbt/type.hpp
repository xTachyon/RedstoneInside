#ifndef REDI_NBT_TYPEE
#define REDI_NBT_TYPEE

#include <ostream>
#include <type_traits>
#include "forward.hpp"

namespace redi {
namespace nbt {

enum class Type : std::uint8_t {
  End,
  Byte,
  Short,
  Int,
  Long,
  Float,
  Double,
  ByteArray,
  String,
  List,
  Compound,
  IntArray,
  ShortArray
};

const char* getNBTTypeName(Type type);
std::ostream& operator<<(std::ostream& stream, Type type);

template <typename T>
struct TypeToNumber {};

template <>
struct TypeToNumber<std::int8_t>
    : public std::integral_constant<Type, Type::Byte> {};

template <>
struct TypeToNumber<std::int16_t>
    : public std::integral_constant<Type, Type::Short> {};

template <>
struct TypeToNumber<std::int32_t>
    : public std::integral_constant<Type, Type::Int> {};

template <>
struct TypeToNumber<std::int64_t>
    : public std::integral_constant<Type, Type::Long> {};

template <>
struct TypeToNumber<float> : public std::integral_constant<Type, Type::Float> {
};

template <>
struct TypeToNumber<double>
    : public std::integral_constant<Type, Type::Double> {};

template <>
struct TypeToNumber<Array<std::int8_t>>
    : public std::integral_constant<Type, Type::ByteArray> {};

template <>
struct TypeToNumber<Array<std::int16_t>>
    : public std::integral_constant<Type, Type::ShortArray> {};

template <>
struct TypeToNumber<Array<std::int32_t>>
    : public std::integral_constant<Type, Type::IntArray> {};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_TYPEE