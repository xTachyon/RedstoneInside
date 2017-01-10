#ifndef REDI_PLAYER
#define REDI_PLAYER

#include "session.hpp"
#include "serverconfig.hpp"

namespace redi
{

class Player
{
public:
  
  const std::int32_t id;
  
  Player(const std::string& name, const std::string uuid, SessionPtr session, std::int32_t id, Server* server,
           Gamemode gamemode = Gamemode::Creative);
  
  Session& getSession() { return *mSession; }
  const Session& getSession() const { return *mSession; }
  Server& getServer() { return *mServer; }
  const Server& getServer() const { return *mServer; }
  Gamemode getGamemode() const { return mGamemode; }
  Dimension getDimension() const { return mDimension; }
  
private:
  
  std::string mNickname;
  std::string mUUID;
  SessionPtr mSession;
  Server* mServer;
  Gamemode mGamemode;
  Dimension mDimension;
};

} // namespace redi

#endif // REDI_PLAYER