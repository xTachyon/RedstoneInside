#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "connectionlistener.hpp"
#include "server.hpp"
#include "util/util.hpp"
#include "protocol/packetwriter.hpp"
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
