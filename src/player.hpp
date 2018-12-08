#pragma once

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

  Session& getSession() { return *session; }
  const Session& getSession() const { return *session; }

  Gamemode getGamemode() const { return gamemode; }
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

  void kick(const std::string& message);
  void kick(std::string&& message);
  
  void kick();

  void disconnect();
  bool isDisconnecting() const { return session->isDisconnecting(); }

  void onTick();

  void onEntityMovedWithLook(PlayerPosition newpos);

  void normalizeRotation();

  void onPositionChanged();
  void onChunkLoaded(world::ChunkHolder& chunk);
  void onUpdateChunks();
  
  Player& getPlayer() override { return *this; }
  Server& getSenderServer() override { return getServer(); }
  
  const std::string& getSenderName() const override { return getUsername(); }
  
  void sendMessageToSender(const std::string& message) override;

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
  SessionSharedPtr session;
  Gamemode gamemode;
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