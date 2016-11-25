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
  
  TagByte a;
  TagShort b;
  TagInt c;
  TagLong d;
  TagFloat e;
  TagDouble f;
  TagString g;
  TagList h;
  TagCompound i;

  TagByteArray x;
  TagIntArray z;

  (&x)->get<NBTType::ByteArray>().push_back(64);

  std::cout << i["123"];

  return 0;
}
