#include <csignal>
#include "server.hpp"

namespace {

// Global variable to server to be able to call it
// from signal function
redi::Server* server{};

// Handles signals and closes the server
// in case of interrupt or any other error
void signalHandler(int signal) {
  switch (signal) {
  case SIGTERM:
  case SIGSEGV:
  case SIGINT:
  case SIGILL:
  case SIGABRT:
  case SIGFPE:
  {
    if (server) {
      server->stop();
    }
  }
  break;

  default:
    break;
  }
}

// Initializes global server variable and
// signals callback.
void installSignals(redi::Server& server) {
  ::server = &server;
  std::signal(SIGTERM, ::signalHandler);
  std::signal(SIGSEGV, ::signalHandler);
  std::signal(SIGINT, ::signalHandler);
  std::signal(SIGILL, ::signalHandler);
  std::signal(SIGABRT, ::signalHandler);
  std::signal(SIGFPE, ::signalHandler);
}

}

int main(int, char**) {
  redi::Logger::debug("Redi is starting");

  try {
    redi::Server server;
    ::installSignals(server);
    server.run();
  } catch (std::exception& e) {
    redi::Logger::error(e.what());
  }

  redi::Logger::info("Redi has stopped");

  return 0;
}
