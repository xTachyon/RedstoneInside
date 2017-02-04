#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "server.hpp"
#include "logger.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  auto a = REDI_FUNCTION;
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