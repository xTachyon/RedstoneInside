#include "roottag.hpp"

namespace redi {
namespace nbt {

void RootTag::writePretty(PrettyPrint& p) const {
  p.writeType(type, name);
  //  PrettyPrint::Block b(p);
  //  TagCompound::writePretty(p);
  p.writeOne(*this);
  TagCompound::writePretty(p);
}

} // namespace nbt
} // namespace redi
