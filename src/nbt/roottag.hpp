#ifndef REDI_NBT_ROOTTAG
#define REDI_NBT_ROOTTAG

#include "compound.hpp"

namespace redi
{
namespace nbt
{

class RootTag : public TagCompound
{
public:

  std::string name;

  RootTag() = default;

  RootTag(RootTag&& other) : name(std::move(other.name)), TagCompound(std::move(other.mData)) {}
};

} // namespace nbt
} // namespace redi


#endif // REDI_NBT_ROOTTAG
