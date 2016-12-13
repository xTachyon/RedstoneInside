#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "protocol/packet.hpp"
#include "logger.hpp"
#include "nbt/nbt.hpp"
#include "region.hpp"
#include "chunk.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;
  
  asio::io_service io;
  asio::ip::tcp::socket s(io);
  asio::ip::tcp::socket c(std::move(s));
  
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
