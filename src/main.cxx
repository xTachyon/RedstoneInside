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
#include <json.hpp>

namespace fs = boost::filesystem;

int main(int, char**)
{
  using namespace redi;
  using namespace redi::protocol;
  using namespace redi::nbt;
  using json = nlohmann::json;
  
  json j;

//  while (true)
//  {
//    std::string name;
//    std::getline(std::cin, name);
//
//    try
//    {
//      std::cout << PrettyPrint(NBTDeserializer(Region(name).readChunk(ChunkPosition(3, 12))));
//    }
//    catch (std::exception& e)
//    {
//      std::cout << e.what() << '\n';
//    }
//  }

  return 0;
}
