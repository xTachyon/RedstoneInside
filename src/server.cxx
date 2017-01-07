#include "server.hpp"

namespace redi
{

void Server::run()
{
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}

} // namespace redi

