#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "protocol/packet.hpp"
#include "logger.hpp"
#include "nbt/nbt.hpp"
#include "region.hpp"
#include "chunk.hpp"
#include "events/event.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;

  std::cout << sizeof(std::vector<Event*>);
  
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
