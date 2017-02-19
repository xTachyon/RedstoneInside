#ifndef REDI_NBT_CREATOR_HPP
#define REDI_NBT_CREATOR_HPP

#include "tag.hpp"
#include "value.hpp"

namespace redi
{
namespace nbt
{

TagUniquePtr create(Type t);
Value createValue(Type t);

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_CREATOR_HPP