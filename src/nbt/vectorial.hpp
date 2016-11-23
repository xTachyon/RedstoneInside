#ifndef REDI_NBT_VECTORIAL
# define REDI_NBT_VECTORIAL

#include "tag.hpp"
#include "compiletimemagic.hpp"

namespace redi
{
namespace nbt
{

template <typename T>
class VectorType : public Tag
{
public:

  static constexpr NBTType type = TagTypeInfo<VectorType<T>>::value;

  VectorType()
    : Tag(type) {}
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_VECTORIAL
