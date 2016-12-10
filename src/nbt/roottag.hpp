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

  RootTag(RootTag&& other) :  TagCompound(std::move(other.mData)), name(std::move(other.name)) {}
  
  using TagCompound::operator[];
};

} // namespace nbt
} // namespace redi


#endif // REDI_NBT_ROOTTAG
