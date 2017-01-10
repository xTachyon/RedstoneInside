#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "connectionlistener.hpp"
#include "server.hpp"
#include "util/util.hpp"
#include "protocol/packetwriter.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;

  asio::io_service ioservice;
  
  Server server(ioservice);
  server.run();
  
  return 0;
}
