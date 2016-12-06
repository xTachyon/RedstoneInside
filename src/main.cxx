#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/multi_array.hpp>
#include "protocol/packet.hpp"
#include "logger.hpp"
#include "nbt/nbt.hpp"
#include "sizeliteraloperators.hpp"
#include "block.hpp"
#include "binarydata.hpp"
#include "compressor.hpp"
#include "region.hpp"
#include "util/util.hpp"
#include "position.hpp"

namespace fs = boost::filesystem;

int main(int, char**)
{
  using namespace redi;
  using namespace redi::protocol;
  using namespace redi::nbt;

  //while (true)
  //{
  //  std::string name;
  //  std::getline(std::cin, name);

  //  try
  //  {
  //    //BinaryData d = Region(name).readChunk(ChunkPosition(-14, 2));
  //    //std::ofstream("s.bin", std::ios::binary).write(reinterpret_cast<const char*>(&d[0]), d.size());
  //    BinaryData d;
  //    d.resize(fs::file_size("s.bin"));
  //    std::ifstream("s.bin", std::ios::binary).read(reinterpret_cast<char*>(&d[0]), d.size());

  //    Region(name).writeChunk(ChunkPosition(3, 15), d);
  //  }
  //  catch (std::exception& e)
  //  {
  //    std::cout << e.what() << '\n';
  //  }
  //}


  return 0;
}
