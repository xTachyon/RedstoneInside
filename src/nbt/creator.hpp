#ifndef REDI_NBT_CREATOR
#	define REDI_NBT_CREATOR

# include "tag.hpp"

namespace redi
{
namespace nbt
{

TagPtr create(NBTType type);
/**
* Create a new tag based on the type
* Do I really need two files just for this ?
*/

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_CREATOR