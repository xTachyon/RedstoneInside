#include <iostream>
#include <fstream>
#include <type_traits>
#include <boost/filesystem.hpp>
#include "protocol/packet.hpp"
#include "logger.hpp"
#include "nbt/nbt.hpp"
#include "literaloperators.hpp"
#include "block.hpp"
#include "binarydata.hpp"

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
  using namespace redi;
  using namespace redi::protocol;
  using namespace redi::nbt;
  
  while (true)
  {
    std::string name;
    std::getline(std::cin, name);

    BinaryData d;
    d.resize(fs::file_size(name));

    std::ifstream f(name);
    f.read(reinterpret_cast<char*>(&d[0]), fs::file_size(name));

    std::cout << Printer(BytesReader(BytesWriter(BytesReader(d)))) << "\n\n";
  }
  
  return 0;
}
