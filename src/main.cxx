#include "server.hpp"

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