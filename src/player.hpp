#ifndef REDI_PLAYER
#define REDI_PLAYER

#include <boost/asio/steady_timer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "session.hpp"
#include "serverconfig.hpp"
#include "playerposition.hpp"

namespace redi
{

class World;

class Player
{
public:
  
  Player(const std::string& name, boost::uuids::uuid uuid, std::unique_ptr<Session>&& session, std::int32_t id, Server* server,
         World* world, Gamemode gamemode = Gamemode::Creative);
  ~Player();
  
  Session& getSession() { return *mSession; }
  const Session& getSession() const { return *mSession; }
  
  Server& getServer() { return *mServer; }
  const Server& getServer() const { return *mServer; }
  
  Gamemode getGamemode() const { return mGamemode; }
  Dimension getDimension() const { return mPosition.dimension; }
  PlayerPosition getPosition() const { return mPosition; }
  
  World& getWorld() { return *mWorld; }
  const World& getWorld() const { return *mWorld; }
  
  const std::string& getPlayerName() const { return mNickname; }
  
  boost::uuids::uuid getUUID() const { return mUUID; }
  std::string getUUIDasString() const { return boost::lexical_cast<std::string>(mUUID); }
  
  std::int32_t getEntityID() const { return mEntityID; }
  
  void sendPacket(ByteBufferSharedPtr ptr);
  void sendMessage(const std::string& message, ChatPosition position = ChatPosition::ChatBox);
  void sendJSONMessage(const std::string& json, ChatPosition position = ChatPosition::ChatBox);
  
  static void onSendKeepAliveTimerRing(const boost::system::error_code& error, boost::asio::steady_timer* timer,
                                        Protocol* protocol);
  
private:
  
  friend class EventManager;
  
  boost::uuids::uuid mUUID;
  std::string mNickname;
  Server* mServer;
  World* mWorld;
  std::unique_ptr<Session> mSession;
  Gamemode mGamemode;
  PlayerPosition mPosition;
  boost::asio::steady_timer mSendKeepAlive;
  const std::int32_t mEntityID;
};

bool operator==(const Player& l, const Player& r);
bool operator!=(const Player& l, const Player& r);

} // namespace redi

#endif // REDI_PLAYER