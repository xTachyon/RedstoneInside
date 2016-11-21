#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include "protocol/packet.hpp"
#include "logger.hpp"
#include "nbt/nbt.hpp"
#include "literaloperators.hpp"
#include "block.hpp"

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
  using namespace redi;
  using namespace redi::protocol;
  using namespace redi::nbt;
  
  TagList list;
  list.push(TagInt(2));
  TagCompound comp;
  comp["12233"] = list;
  comp["7809"] = create(NBTType::List);

  return 0;
}
