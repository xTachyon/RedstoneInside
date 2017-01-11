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
  
  Player(const std::string& name, const std::string uuid, Session* session, std::int32_t id, Server* server,
           Gamemode gamemode = Gamemode::Creative);
  ~Player();
  
  Session& getSession() { return *mSession; }
  const Session& getSession() const { return *mSession; }
  Server& getServer() { return *mServer; }
  Session* getSessionPtr() const { return mSession; }
  const Server& getServer() const { return *mServer; }
  Gamemode getGamemode() const { return mGamemode; }
  Dimension getDimension() const { return mDimension; }
  Vector3d getPosition() const { return mPosition; }
  
private:
  
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
  
  std::string mNickname;
  std::string mUUID;
  Session* mSession;
  Server* mServer;
  Gamemode mGamemode;
  Dimension mDimension;
  Vector3d mPosition;
  boost::asio::deadline_timer mSendKeepAlive;
  //boost::asio::deadline_timer mReceivedKeepAlive;
  //TimePoint mLastReceivedKeepAlive;
  
  void onSendKeepAliveTimerRing();
};

} // namespace redi

#endif // REDI_PLAYER