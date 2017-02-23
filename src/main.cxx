#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "server.hpp"
#include "nbt/primitive.hpp"
#include "nbt/nbt.hpp"
#include "compressor.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;
  using namespace redi::nbt;
  using namespace std::string_literals;
  
  while (true)
  {
    std::string path;
    std::getline(std::cin, path);
    if (path == "*") break;
    
    world::AnvilRegion r(path);
    
    ByteBuffer b = r.readChunk(Vector2i(13, -20));
    RootTag root;
    Deserializer(b).read(root);
    
    b.clear();
    Serializer(b).write(root);
    r.writeChunk(Vector2i(4, -11), b);
    
  } // /home/andrei/.minecraft/saves/redi/region/r.0.-1.mca
  
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