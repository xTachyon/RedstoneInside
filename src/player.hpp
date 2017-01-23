#ifndef REDI_PLAYER
#define REDI_PLAYER

#include "session.hpp"
#include "serverconfig.hpp"

namespace redi
{

class World;

class Player
{
public:
  
  const std::int32_t id;
  
  Player(const std::string& name, const std::string uuid, Session* session, std::int32_t id, Server* server, World* world,
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
  World& getWorld() { return *mWorld; }
  const World& getWorld() const { return *mWorld; }
  void sendPacket(ByteBufferSharedPtr ptr);
  const std::string& getPlayerName() const { return mNickname; }
  
  static void onSendKeepAliveTimerRing(const boost::system::error_code& error, boost::asio::deadline_timer* timer,
                                        Protocol* protocol);
  
  
  private:
  
  std::string mNickname;
  std::string mUUID;
  Session* mSession;
  Server* mServer;
  Gamemode mGamemode;
  Dimension mDimension;
  Vector3d mPosition;
  boost::asio::deadline_timer mSendKeepAlive;
  World* mWorld;
};

bool operator==(const Player& l, const Player& r);
bool operator!=(const Player& l, const Player& r);

} // namespace redi

#endif // REDI_PLAYER