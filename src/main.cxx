#include <iostream>
#include <fstream>
#include <type_traits>
#include <boost/filesystem.hpp>
#include "protocol/packet.hpp"
#include "logger.hpp"
#include "nbt/nbt.hpp"
#include "sizeliteraloperators.hpp"
#include "block.hpp"
#include "binarydata.hpp"
#include "compressor.hpp"

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
  using namespace redi;
  using namespace redi::protocol;
  using namespace redi::nbt;
  
  BinaryData e = reinterpret_cast<const std::uint8_t*>("abcdefghijklmnopqrstuvwxyz1234567891011121314151617181920212223242526272829303132");
  e = compressor::compresszlib(e);
  e = compressor::decompresszlib(e);
  
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
