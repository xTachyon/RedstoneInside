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
    std::string s;
    std::getline(std::cin, s);
    if (s == "*") return 0;
    
    redi::ByteBuffer buffer;
    buffer.resize(fs::file_size(s));
    std::ifstream(s).read(buffer.as_char(), buffer.size());
    buffer = compressor::decompressGzip(buffer);
    
    std::string name;
    TagCompound root;
    Deserializer(buffer).read(name, root);
    
    buffer.clear();
    Serializer(buffer).write(name, root);
    buffer = compressor::compressGzip(buffer);
    std::ofstream(s + "_c"s).write(buffer.as_const_char(), buffer.size());
    
    std::cout << 1;
  }
  
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