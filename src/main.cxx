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
  
  RootTag root, nested;
  root.name = "root abc";
  root[1] = TagInt(6);
  root[2] = TagDouble(87.3426);
  nested[5] = 6;
  nested[0] = "7654";
  root[90] = nested;

  std::string str = Printer(root);

  std::cout << str;

  return 0;
}
