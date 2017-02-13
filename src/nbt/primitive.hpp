#ifndef REDI_NBT_PRIMITIVE_HPP
#define REDI_NBT_PRIMITIVE_HPP

#include "basic.hpp"

namespace redi
{
namespace nbt
{

template <typename T>
struct Primitive : public BasicTag<Primitive<T>>
{
  static constexpr Type type = TypeToNumber<T>::value;
  
  T data;
  
  Primitive(T value = 0) : data(value) {}
  
  Primitive& operator=(T value)
  {
    data = value;
    return *this;
  }
  
  operator T&() { return data; }
  operator T() const { return data; }
};

using TagByte = Primitive<std::int8_t>;
using TagShort = Primitive<std::int16_t>;
using TagInt = Primitive<std::int32_t>;
using TagLong = Primitive<std::int64_t>;
using TagFloat = Primitive<float>;
using TagDouble = Primitive<double>;
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_PRIMITIVE_HPP