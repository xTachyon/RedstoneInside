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
  
  TagCompound tag;
  tag["abc"] = 123;
  tag["123"] = "790";
  std::cout << tag["123"];

  return 0;
}
