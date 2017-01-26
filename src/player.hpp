#ifndef REDI_PLAYER
#define REDI_PLAYER

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "session.hpp"
#include "serverconfig.hpp"

namespace redi
{

class World;

class Player
{
public:
  
  
  Player(const std::string& name, boost::uuids::uuid uuid, Session* session, std::int32_t id, Server* server,
         World* world,
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
  std::string getUUID() const { return boost::lexical_cast<std::string>(mUUID); }
  std::int32_t getEntityID() const { return mEntityID; }
  
  static void onSendKeepAliveTimerRing(const boost::system::error_code& error, boost::asio::deadline_timer* timer,
                                        Protocol* protocol);
  
  private:
  
  boost::uuids::uuid mUUID;
  std::string mNickname;
  Server* mServer;
  World* mWorld;
  Session* mSession;
  Gamemode mGamemode;
  Dimension mDimension;
  Vector3d mPosition;
  boost::asio::deadline_timer mSendKeepAlive;
  const std::int32_t mEntityID;
};

bool operator==(const Player& l, const Player& r);
bool operator!=(const Player& l, const Player& r);

} // namespace redi

#endif // REDI_PLAYER