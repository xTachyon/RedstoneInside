#ifndef REDI_NBT_TYPE
# define REDI_NBT_TYPE

#include <boost/format.hpp>

namespace redi
{
namespace nbt
{

enum class Type : std::uint8_t
{
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
  IntArray
};

const char* getNBTTypeName();
std::ostream& operator<<(std::ostream& stream, Type type);

template <typename T>
struct TypeToNumber {};

template <>
struct TypeToNumber<std::int8_t> : public std::integral_constant<Type, Type::Byte> {};

template <>
struct TypeToNumber<std::int16_t> : public std::integral_constant<Type, Type::Short> {};

template <>
struct TypeToNumber<std::int32_t> : public std::integral_constant<Type, Type::Int> {};

template <>
struct TypeToNumber<std::int64_t> : public std::integral_constant<Type, Type::Long> {};

template <>
struct TypeToNumber<float> : public std::integral_constant<Type, Type::Float> {};

template <>
struct TypeToNumber<double> : public std::integral_constant<Type, Type::Double> {};

/*
 *
    template<> struct get_primitive_type<int8_t>  : public std::integral_constant<tag_type, tag_type::Byte> {};
    template<> struct get_primitive_type<int16_t> : public std::integral_constant<tag_type, tag_type::Short> {};
    template<> struct get_primitive_type<int32_t> : public std::integral_constant<tag_type, tag_type::Int> {};
    template<> struct get_primitive_type<int64_t> : public std::integral_constant<tag_type, tag_type::Long> {};
    template<> struct get_primitive_type<float>   : public std::integral_constant<tag_type, tag_type::Float> {};
    template<> struct get_primitive_type<double>  : public std::integral_constant<tag_type, tag_type::Double> {};
}
 */

} // namespace nbt
} // namespace redi


#endif // REDI_NBT_TYPE