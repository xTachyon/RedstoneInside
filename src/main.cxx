#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/multi_array.hpp>
#include <boost/lockfree/queue.hpp>
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
#include "chunk.hpp"
#include <boost/optional.hpp>
#include <boost/asio.hpp>
#include "threadsafequeue.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;
  using namespace redi::protocol;
  using namespace redi::nbt;
  
  boost::lockfree::queue<int> q;
  
  
  while (true)
  {
    std::string name;
    std::getline(std::cin, name);
    if (name == "*") break;

    try
    {
      Chunk c(Region(name).readChunk(Vector2(3, 12)));
    }
    catch (std::exception& e)
    {
      std::cout << e.what() << '\n';
    }
  }

  return 0;
}
