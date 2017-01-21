#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "server.hpp"
#include "logger.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  asio::io_service io_service;

  try
  {
    redi::Server server(io_service);
    server.run();
  }
  catch (std::exception& e)
  {
    redi::Logger::error(e.what());
  }

  return 0;
}