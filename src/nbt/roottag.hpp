#ifndef REDI_NBT_ROOTTAG_HPP
#define REDI_NBT_ROOTTAG_HPP

#include "compound.hpp"

namespace redi {
namespace nbt {

struct RootTag : public TagCompound {
  std::string name;

  using TagCompound::TagCompound;
  using TagCompound::operator=;
  using TagCompound::operator[];

  void writePretty(PrettyPrint& p) const;
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_ROOTTAG_HPP