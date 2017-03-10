#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "server.hpp"
#include "nbt/primitive.hpp"
#include "nbt/nbt.hpp"
#include "compressor.hpp"
#include "world/anvil.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;
  using namespace redi::nbt;
  using namespace std::string_literals;
  
//  while (true)
//  {
//    std::string path;
//    if (path == "*")
//    {
//      break;
//    }
//
//    int x, z;
//    std::cin >> x >> z;
//    std::string p = (boost::format("/home/andrei/.minecraft/saves/redi/region/r.%1%.%2%.mca") % x % z).str();
//
//    world::Anvil a(p, Vector2i(x, z));
//
//    ByteBuffer buffer;
//    auto xz = a.readChunk(Vector2i(-1, -10), buffer);
//    std::ofstream("in.data").write(buffer.as_const_char(), buffer.size());
//
//    a.writeChunk(Vector2i(-1, -10), buffer);
//
//    std::cout;
//  }
  
  redi::Logger::debug("Redi is starting");
  
  try
  {
    redi::Server server;
    server.run();
  }
  catch (std::exception& e)
  {
    redi::Logger::error(e.what());
  }
  
  redi::Logger::info("Redi has stopped");

  return 0;
}