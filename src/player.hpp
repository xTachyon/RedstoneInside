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
  
  Player(const std::string& name, boost::uuids::uuid uuid, std::shared_ptr<Session> session, std::int32_t id, Server* server,
         World* world, Gamemode gamemode = Gamemode::Creative);
  Player(const Player&) = delete;
  Player(Player&&) = delete;
  ~Player();
  
  Player& operator=(const Player&) = delete;
  Player& operator=(Player&&) = delete;
  
  Session& getSession() { return *mSession; }
  const Session& getSession() const { return *mSession; }
  
  Server& getServer() { return *mServer; }
  const Server& getServer() const { return *mServer; }
  
  Gamemode getGamemode() const { return mGamemode; }
  Dimension getDimension() const { return mPosition.dimension; }
  PlayerPosition getPosition() const { return mPosition; }
  
  World& getWorld() { return *mWorld; }
  const World& getWorld() const { return *mWorld; }
  
  const std::string& getUsername() const { return mNickname; }
  
  boost::uuids::uuid getUUID() const { return mUUID; }
  std::string getUUIDasString() const { return boost::lexical_cast<std::string>(mUUID); }
  
  std::int32_t getEntityID() const { return mEntityID; }
  std::size_t getNewTeleportID() { return mTeleportID++; }
  
  void sendPacket(ByteBufferSharedPtr ptr);
  void sendMessage(const std::string& message, ChatPosition position = ChatPosition::ChatBox);
  void sendJSONMessage(const std::string& json, ChatPosition position = ChatPosition::ChatBox);
  
  void kickJSONmessage(const std::string& json);
  void kickJSONmessage(std::string&& json);
  void kick(const std::string& message);
  void kick(std::string&& message);
  
  bool isDisconnecting() const { return mSession->isDisconnecting(); }
  
  static void onSendKeepAliveTimerRing(const boost::system::error_code& error, boost::asio::steady_timer* timer, SessionSharedPtr session);
  
private:
  
  friend class EventManager;
  friend class PacketHandler;
  
  boost::uuids::uuid mUUID;
  std::string mNickname;
  Server* mServer;
  World* mWorld;
  SessionSharedPtr mSession;
  Gamemode mGamemode;
  PlayerPosition mPosition;
  boost::asio::steady_timer mSendKeepAlive;
  std::size_t mTeleportID;
  const std::int32_t mEntityID;
  
  std::string getPlayerDataFileName() const;
  void saveToFile();
  void loadFromFile();
};

bool operator==(const Player& l, const Player& r);
bool operator!=(const Player& l, const Player& r);

} // namespace redi

#endif // REDI_PLAYER