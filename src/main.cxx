#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "server.hpp"
#include "nbt/primitive.hpp"
#include "nbt/nbt.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi::nbt;
  
//  redi::nbt::TagCompound c;
//
//  c["123"] = 7635.4321;
//  c["123"] = 13.54;
//  c["123"] = 7635;
  
  Value v("abc");
  Value c = v;
  Value m = std::move(v);
  
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