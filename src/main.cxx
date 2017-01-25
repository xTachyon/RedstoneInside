#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "server.hpp"
#include "logger.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  auto a = sizeof(redi::Session);
  auto b = sizeof(redi::Server);
  auto c = sizeof(redi::Player);
  auto d = sizeof(redi::ConnectionListener);
  auto e = sizeof(std::atomic_bool);
  
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