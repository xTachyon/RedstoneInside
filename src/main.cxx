#include "server.hpp"
#include "nbt/primitive.hpp"
#include "nbt/nbt.hpp"
#include "compressor.hpp"
#include "buffers.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;
using namespace redi;

int main(int, char**) {
  redi::Logger::debug("Redi is starting");

  try {
    redi::Server server;
    server.run();
  } catch (std::exception& e) {
    redi::Logger::error(e.what());
  }

  redi::Logger::info("Redi has stopped");

  return 0;
}