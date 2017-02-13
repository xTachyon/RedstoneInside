#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "server.hpp"
#include "nbt/primitive.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  redi::nbt::TagInt i;
  i = 654;
  auto a = i.getType();
  
  try
  {
    redi::Server server;
    server.run();
  }
  catch (std::exception& e)
  {
    redi::Logger::error(e.what());
  }

  return 0;
}