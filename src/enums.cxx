#include <cstdlib>
#include "enums.hpp"

namespace redi
{

const char* getStateName(ConnectionState s)
{
  const char* names[] = {"Handshake", "Status", "Login", "Play"};
  return names[static_cast<std::size_t>(s)];
}
  
} // namespace redi
