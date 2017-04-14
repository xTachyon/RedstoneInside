#ifndef REDI_PLAYER
#define REDI_PLAYER

#include <list>
#include <boost/asio/steady_timer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <set>
#include "session.hpp"
#include "serverconfig.hpp"
#include "playerposition.hpp"
#include "world/memoryregion.hpp"
#include "commands/commandsender.hpp"

namespace redi {

class Player;
class World;

using PlayerSharedPtr = std::shared_ptr<Player>;

class Player : public HasServer, public commands::CommandSender, public std::enable_shared_from_this<Player> {
public:
  Player(const std::string& name, boost::uuids::uuid uuid,
         std::shared_ptr<Session> session, std::int32_t id, Server& server,
         World* world, Gamemode gamemode = Gamemode::Creative);
  Player(const Player&) = delete;
  Player(Player&&) = delete;
  ~Player();

  Player& operator=(const Player&) = delete;
  Player& operator=(Player&&) = delete;

  Session& getSession() { return *mSession; }
  const Session& getSession() const { return *mSession; }

  Gamemode getGamemode() const { return mGamemode; }
  Dimension getDimension() const { return mPosition.dimension; }
  PlayerPosition getPosition() const { return mPosition; }

  World& getWorld() { return *mWorld; }
  const World& getWorld() const { return *mWorld; }

  const std::string& getUsername() const { return mNickname; }

  boost::uuids::uuid getUUID() const { return mUUID; }
  std::string getUUIDasString() const {
    return boost::lexical_cast<std::string>(mUUID);
  }

  std::int32_t getEntityID() const { return mEntityID; }
  std::int32_t getNewTeleportID() { return mTeleportID++; }

  void sendPacket(const ByteBuffer& packet);
  void sendPacket(ByteBuffer&& packet);
  void sendMessage(const std::string& message,
                   ChatPosition position = ChatPosition::ChatBox);
  void sendJSONMessage(const std::string& json,
                       ChatPosition position = ChatPosition::ChatBox);

  void kickJSONmessage(const std::string& json);
  void kickJSONmessage(std::string&& json);
  void kick(const std::string& message);
  void kick(std::string&& message);

  void disconnect();
  bool isDisconnecting() const { return mSession->isDisconnecting(); }

  void onTick();

  void onEntityMovedWithLook(PlayerPosition newpos);

  void normalizeRotation();

  void onPositionChanged();
  void onChunkLoaded(world::ChunkHolder& chunk);
  void onUpdateChunks();
  
  Player& getPlayer() override { return *this; }
  Server& getSenderServer() override { return getServer(); }
  
  string_view getSenderName() const override { return getUsername(); }
  
  void sendMessageToSender(string_view message) override { sendMessage(message.to_string()); }

private:
  friend class EventManager;
  friend class PacketHandler;
  friend class Session;

  static constexpr double InRange = 16 * 10.0;

  std::vector<Player*> mEntitiesInSight;
  Vector2i mLastPositionWhenChunksWasSent;
  std::list<world::ChunkHolder> loadedChunks;

  boost::uuids::uuid mUUID;
  std::string mNickname;
  World* mWorld;
  SessionSharedPtr mSession;
  Gamemode mGamemode;
  PlayerPosition mPosition;
  boost::asio::steady_timer mSendKeepAliveTimer;
  std::int32_t mTeleportID;
  const std::int32_t mEntityID;
  std::atomic_bool hasSavedToDisk;

  std::string getPlayerDataFileName() const;
  void saveToFile();
  void loadFromFile();

  void onSendKeepAliveTimer(const boost::system::error_code& error);
  void keepAliveNext();

  void timersNext();
};

bool operator==(const Player& l, const Player& r);
bool operator!=(const Player& l, const Player& r);

} // namespace redi

#endif // REDI_PLAYER