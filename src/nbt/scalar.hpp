#ifndef REDI_NBT_SCALAR
# define REDI_NBT_SCALAR

# include "tag.hpp"
# include "type.hpp"
# include "compiletimemagic.hpp"

namespace redi
{
namespace nbt
{

template <typename T>
class ScalarTag : public Tag
{
public:

  T data;

  ScalarTag(T data = 0)
    : Tag(type), data(data) {}

  ScalarTag& operator=(T data)
  {
    this->data = data;
    return *this;
  }

  operator T&() { return data; }
  operator const T&() const { return data; }

  static constexpr NBTType type = TagTypeInfo<T>::value;

  TagPtr clone() const override { return std::make_shared<ScalarTag>(*this); }

protected:

  bool equals(const Tag& r) const override
  {
    return *this == dynamic_cast<const ScalarTag&>(r);
  }

};

template <typename T, typename K>
bool operator==(const ScalarTag<T>& lhs, const ScalarTag<K>& rhs)
{
  return lhs.data == rhs.data;
}

template <typename T, typename K>
bool operator!=(const ScalarTag<T>& lhs, const ScalarTag<K>& rhs)
{
  return lhs.data != rhs.data;
}

template <typename T, typename K>
bool operator==(const ScalarTag<T>& lhs, K rhs)
{
  static_assert(std::is_arithmetic<K>::value, "Type must be arithmetic");
  return lhs.data == rhs;
}
template <typename T, typename K>
bool operator!=(const ScalarTag<T>& lhs, K rhs)
{
  static_assert(std::is_arithmetic<K>::value, "Type must be arithmetic");
  return lhs.data != rhs;
}

using TagByte = ScalarTag<std::int8_t>;
using TagShort = ScalarTag<std::int16_t>;
using TagInt = ScalarTag<std::int32_t>;
using TagLong = ScalarTag<std::int64_t>;
using TagFloat = ScalarTag<float>;
using TagDouble = ScalarTag<double>;

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_SCALAR