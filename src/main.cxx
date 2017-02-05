#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "server.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
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