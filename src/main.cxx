#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "connectionlistener.hpp"
#include "server.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;

  asio::io_service ioservice;
  
  std::shared_ptr<int> p(new int(5));
  p = {};
  
  Server server(ioservice);
  server.run();
  
  return 0;
}
