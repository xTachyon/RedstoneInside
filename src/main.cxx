#include "server.hpp"
#include "nbt/primitive.hpp"
#include "nbt/nbt.hpp"
#include "compressor.hpp"
#include "buffers.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**) {
  using namespace redi;
  using namespace redi::nbt;
  using namespace std::string_literals;
  
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