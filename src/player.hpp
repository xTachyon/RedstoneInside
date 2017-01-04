#ifndef REDI_PLAYER
#define REDI_PLAYER

#include "session.hpp"

namespace redi
{

class Player
{
public:
  
  Session& getSession() { return mSession; }
  
private:
  
  Session mSession;
  std::string mNickname;
  std::string mUUID;
};

} // namespace redi

#endif // REDI_PLAYER