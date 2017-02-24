#ifndef REDI_NBT_PRIMITIVE_HPP
#define REDI_NBT_PRIMITIVE_HPP

#include "basic.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "prettyprinter.hpp"

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

  void write(Serializer& s) const override
  {
    s.writeNumber(data);
  }

  void read(Deserializer& s) override
  {
    data = s.readNumber<T>();
  }

  void writePretty(PrettyPrint& p) const override
  {
    p.string += std::to_string(data);
  }
};

template <>
inline void Primitive<std::int8_t>::writePretty(PrettyPrint& p) const
{
  p.string += std::to_string(static_cast<int>(data));
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Primitive<T>& obj)
{
  stream << obj.toString();
  return stream;
}

using TagByte = Primitive<std::int8_t>;
using TagShort = Primitive<std::int16_t>;
using TagInt = Primitive<std::int32_t>;
using TagLong = Primitive<std::int64_t>;
using TagFloat = Primitive<float>;
using TagDouble = Primitive<double>;

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_PRIMITIVE_HPP
