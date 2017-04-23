#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "player.hpp"
#include "server.hpp"
#include "logger.hpp"
#include "util/util.hpp"
#include "protocol/packets/server/play/keepalive.hpp"
#include "protocol/packets/server/play/disconnect.hpp"
#include "protocol/packets/server/play/spawnplayer.hpp"
#include "protocol/packets/server/play/chunkdata.hpp"
#include "protocol/packets/server/play/unloadchunk.hpp"

namespace asio = boost::asio;
namespace fs = boost::filesystem;

namespace redi {

Player::Player(const std::string& name, boost::uuids::uuid uuid,
               std::shared_ptr<Session> session, std::int32_t id,
               Server& server, World* world, Gamemode gamemode)
    : HasServer(server), CommandSender(commands::CommandSenderType::Player), mUUID(uuid), mNickname(name),
      mWorld(world),
      mSession(session), mGamemode(gamemode),
      mSendKeepAliveTimer(mSession->getIoService()), mTeleportID(0),
      mEntityID(id), hasSavedToDisk(false) {
  Logger::debug((boost::format("Player %1% created") % this).str());

  loadFromFile();

  mLastPositionWhenChunksWasSent.x = 0;
  mLastPositionWhenChunksWasSent.z = 0;
}

Player::~Player() {
  Logger::debug((boost::format("Player %1% destroyed") % this).str());
  
  bool saved = hasSavedToDisk.exchange(true);
  
  if (!saved) {
    saveToFile();
  }
}

void Player::onSendKeepAliveTimer(const boost::system::error_code& error) {
  using namespace std::chrono_literals;

  if (!error && !mSession->isDisconnecting()) {
    packets::KeepAlive(5).send(*mSession);

    mSendKeepAliveTimer.expires_from_now(15s);
    keepAliveNext();
  }
}

void Player::keepAliveNext() {
  mSendKeepAliveTimer.async_wait(boost::bind(&Player::onSendKeepAliveTimer,
                                             shared_from_this(),
                                             asio::placeholders::error));
}

void Player::sendPacket(const ByteBuffer& packet) {
  mSession->sendPacket(packet);
}

void Player::sendPacket(ByteBuffer&& packet) {
  mSession->sendPacket(std::move(packet));
}

void Player::sendMessage(const std::string& message, ChatPosition position) {
  sendJSONMessage(message, position);
}

void Player::sendMessageToSender(const std::string& message) {
  // TODO: come back here
  std::string s(message);
  sendMessage(s);
}

void Player::sendJSONMessage(const std::string& json, ChatPosition position) {
  packets::ChatMessage(json, position).send(*&*mSession);
}

void Player::kickJSONmessage(const std::string& json) {
  kickJSONmessage(std::string(json));
}

void Player::kickJSONmessage(std::string&& json) {
  packets::Disconnect(std::move(json)).send(*mSession);
  disconnect();
}

void Player::kick(std::string&& message) {
  kickJSONmessage(std::move(message));
}

void Player::disconnect() {
  mSendKeepAliveTimer.cancel();

  mSession->disconnect();
  
  if (!hasSavedToDisk) {
    hasSavedToDisk = true;
    saveToFile();
  }
}

void Player::normalizeRotation() {
  mPosition.yaw = util::normalizeAngleDegrees(mPosition.yaw);
  mPosition.pitch = util::normalizeAngleDegrees(mPosition.pitch);
}

void Player::kick(const std::string& message) { kick(std::string(message)); }

std::string Player::getPlayerDataFileName() const {
  return "players/" + getUUIDasString() + ".json";
}

void Player::loadFromFile() try {
  if (!fs::exists(getPlayerDataFileName())) {
    mPosition.y = 50;
    return;
  }

  nlohmann::json j =
      nlohmann::json::parse(util::readFileToString(getPlayerDataFileName()));

  {
    nlohmann::json& position = j["position"];
    mPosition.x = position[0];
    mPosition.y = position[1];
    mPosition.z = position[2];
  }

  {
    nlohmann::json& rotation = j["rotation"];
    mPosition.yaw = rotation[0];
    mPosition.pitch = rotation[1];
  }

  mPosition.onGround = j["onground"];

  Logger::debug((boost::format("Player %1% loaded from file") % this).str());
} catch (std::exception&) {
}

void Player::saveToFile() {
  nlohmann::json j;

  j["world"] = mWorld->getWorldName();

  {
    nlohmann::json& position = j["position"];
    position.push_back(mPosition.x);
    position.push_back(mPosition.y);
    position.push_back(mPosition.z);
  }

  {
    nlohmann::json& rotation = j["rotation"];
    rotation.push_back(mPosition.yaw);
    rotation.push_back(mPosition.pitch);
  }

  j["onground"] = mPosition.onGround;
  j["name"] = mNickname;

  std::ofstream(getPlayerDataFileName()) << j.dump(2);
  Logger::debug((boost::format("Player %1% saved to file") % this).str());
}

bool operator==(const Player& l, const Player& r) { return &l == &r; }

bool operator!=(const Player& l, const Player& r) { return !(l == r); }

void Player::onEntityMovedWithLook(PlayerPosition newpos) {
  using PlayerPtrVector = std::vector<Player*>;
  PlayerPtrVector create;
  PlayerPtrVector update;
  PlayerPtrVector destroy;
  PlayerPtrVector nextEntitiesInSight;
  
  for (PlayerSharedPtr& player : server.getOnlinePlayers()) {
    if (*player != *this) {
      auto distance = player->getPosition().distance(mPosition);

      if (distance <= 32 * 10.0) {
        auto it = std::find(mEntitiesInSight.begin(), mEntitiesInSight.end(),
                            player.get());

        if (it == mEntitiesInSight.end()) {
          create.push_back(player.get());
          nextEntitiesInSight.push_back(player.get());
        } else {
          update.push_back(player.get());
          nextEntitiesInSight.push_back(player.get());
        }
      } else {
        destroy.push_back(player.get());
      }
    }
  }

  if (create.size() > 0) {
    packets::SpawnPlayer packet(*this);

    for (Player* player : create) {
      packet.send(*player);
    }
  }

  if (update.size() > 0) {
    std::int16_t dx = (util::floorAndCast(newpos.x * 32.0) -
                       util::floorAndCast(mPosition.x * 32.0)) *
                      128;
    std::int16_t dy = (util::floorAndCast(newpos.y * 32.0) -
                       util::floorAndCast(mPosition.y * 32.0)) *
                      128;
    std::int16_t dz = (util::floorAndCast(newpos.z * 32.0) -
                       util::floorAndCast(mPosition.z * 32.0)) *
                      128;

    packets::EntityLookAndRelativeMove packet(mEntityID, dx, dy, dz,
                                              mPosition.yaw, mPosition.pitch,
                                              mPosition.onGround);

    for (Player* player : update) {
      packet.send(*player);
    }
  }

  mEntitiesInSight = nextEntitiesInSight;
}

void Player::timersNext() { keepAliveNext(); }

void Player::onPositionChanged() {
  if (Vector2i(static_cast<std::int32_t>(mPosition.x / 16),
               static_cast<std::int32_t>(mPosition.z / 16))
          .distanceSquared(mLastPositionWhenChunksWasSent) > 1) {
    onUpdateChunks();
  }
}

void Player::onChunkLoaded(world::ChunkHolder& chunk) {
  packets::ChunkData(*chunk, chunk.getCoords()).send(mSession);
  loadedChunks.push_back(chunk);
}

void Player::onUpdateChunks() {
  auto range = getServer().config.rangeView;

  Vector2i playerchunk(static_cast<std::int32_t>(mPosition.x / 16),
                       static_cast<std::int32_t>(mPosition.z / 16));

  Vector2i start(playerchunk.x - range, playerchunk.z - range - 1);
  Vector2i end(playerchunk.x + range + 1, playerchunk.z + range);

  std::list<world::ChunkHolder> newchunks;
  world::ChunkManager& cm = mWorld->getChunkManager();

  for (std::int32_t x = start.x; x < end.x; ++x) {
    for (std::int32_t z = start.z; z < end.z; ++z) {
      Vector2i th(x, z);

      auto it = loadedChunks.end();
      for (auto i = loadedChunks.begin(); i != it; ++i) {
        if (i->getCoords() == th) {
          it = i;
          break;
        }
      }

      if (it == loadedChunks.end()) {
        if (cm.isChunkLoaded(th)) {
          world::ChunkHolder chunk = cm(th);
          newchunks.push_back(chunk);
          packets::ChunkData(*chunk, chunk.getCoords()).send(mSession);
        } else {
          cm.loadChunk(th, shared_from_this());
        }
      } else {
        world::ChunkHolder chunk = cm(th);
        newchunks.push_back(chunk);
        loadedChunks.remove_if([&chunk](const world::ChunkHolder& l) {
          return l.getCoords() == chunk.getCoords();
        });
      }
    }
  }

  for (const world::ChunkHolder& chunk : loadedChunks) {
    packets::UnloadChunk(chunk.getCoords()).send(mSession);
  }

  loadedChunks = std::move(newchunks);
  mLastPositionWhenChunksWasSent = mPosition.getChunkPosition();
}

} // namespace redi
