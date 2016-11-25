#ifndef REDI_NBT_VECTORIAL
# define REDI_NBT_VECTORIAL

# include <vector>
# include "tag.hpp"
# include "compiletimemagic.hpp"

namespace redi
{
namespace nbt
{

template <typename T>
class VectorType : public Tag
{
public:

  static constexpr NBTType type = TagTypeInfo<VectorType<T>>::value;

  using Container = std::vector<T>;

  Container data;

  VectorType()
    : Tag(type) {}

  operator Container&() { return data; }
  operator const Container&() const { return data; }

  TagPtr clone() const override { return TagPtr(new VectorType(*this)); }

protected:

  bool equals(const Tag& r) const override
  {
    return *this == dynamic_cast<const VectorType&>(r);
  }
};

using TagByteArray = VectorType<std::int8_t>;
using TagIntArray = VectorType<std::int32_t>;

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_VECTORIAL
