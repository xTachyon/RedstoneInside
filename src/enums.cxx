#include "enums.hpp"

namespace redi
{

const char* getStateName(ConnectionState s)
{
  const char* names[] = {"Handshake", "Status", "Login", "Play"};
  return names[static_cast<std::size_t>(s)];
  
  const char* ptr;
  
  switch (s)
  {
  case ConnectionState::Play:
    ptr = "Play";
    break;
  
  case ConnectionState::Status:
    ptr = "Status";
    break;
  
  case ConnectionState::Login:
    ptr = "Login";
    break;
  
  case ConnectionState::Handshake:
    ptr = "Handshake";
    break;
  }
  
  return ptr;
}
  
} // namespace redi
