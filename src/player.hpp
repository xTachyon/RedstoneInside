#ifndef REDI_PLAYER
#define REDI_PLAYER

#include "session.hpp"

namespace redi
{

class Player
{
public:
  
  Player(const std::string& name, const std::string uuid, SessionPtr session);
  
  Session& getSession() { return *mSession; }
  
private:
  
  std::string mNickname;
  std::string mUUID;
  SessionPtr mSession;
  
};

} // namespace redi

#endif // REDI_PLAYER