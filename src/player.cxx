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

namespace redi
{

Player::Player(const std::string& name, boost::uuids::uuid uuid, std::shared_ptr<Session> session, std::int32_t id, Server* server,
               World* world, Gamemode gamemode)
    : mUUID(uuid), mNickname(name), mServer(server), mWorld(world), mSession(session), mGamemode(gamemode), mSendKeepAliveTimer(mSession->getIoService()),
      mUpdateChunksTimer(mSession->getIoService()), mTeleportID(0), mEntityID(id), mHasSavedToDisk(false)
{
  Logger::debug((boost::format("Player %1% created") % this).str());
  
  loadFromFile();
}

Player::~Player()
{
  Logger::debug((boost::format("Player %1% destroyed") % this).str());
  
  if (!mHasSavedToDisk)
  {
    mHasSavedToDisk = true;
    saveToFile();
  }
}

void Player::onSendKeepAliveTimer(const boost::system::error_code& error)
{
  using namespace std::chrono_literals;
  
  if (!error && !mSession->isDisconnecting())
  {
    packets::KeepAlive(5).send(*mSession);
    
    mSendKeepAliveTimer.expires_from_now(15s);
    keepAliveNext();
  }
}

void Player::keepAliveNext()
{
  mSendKeepAliveTimer.async_wait(boost::bind(&Player::onSendKeepAliveTimer, shared_from_this(), asio::placeholders::error));
}

void Player::sendPacket(const ByteBuffer& packet)
{
  mSession->sendPacket(packet);
}

void Player::sendPacket(ByteBuffer&& packet)
{
  mSession->sendPacket(std::move(packet));
}

void Player::sendMessage(const std::string& message, ChatPosition position)
{
  sendJSONMessage(ChatManager::componentToJson(ChatComponent(
        {
              ChatMessagePart(message)
        })), position);
}

void Player::sendJSONMessage(const std::string& json, ChatPosition position)
{
  packets::ChatMessage(json, position).send(*&*mSession);
}

void Player::kickJSONmessage(const std::string& json)
{
  kickJSONmessage(std::string(json));
}

void Player::kickJSONmessage(std::string&& json)
{
  packets::Disconnect(std::move(json)).send(*mSession);
  disconnect();
}

void Player::kick(std::string&& message)
{
  kickJSONmessage(ChatManager::componentToJson(ChatComponent(
        {
              ChatMessagePart(std::move(message))
        })));
}

void Player::disconnect()
{
  mSendKeepAliveTimer.cancel();
  mUpdateChunksTimer.cancel();
  
  mSession->disconnect();
  
  if (!mHasSavedToDisk)
  {
    mHasSavedToDisk = true;
    saveToFile();
  }
}

void Player::normalizeRotation()
{
  mPosition.yaw = static_cast<float>(util::normalizeAngleDegrees(mPosition.yaw));
  mPosition.pitch = static_cast<float>(util::normalizeAngleDegrees(mPosition.pitch));
}

void Player::kick(const std::string& message)
{
  kick(std::string(message));
}

std::string Player::getPlayerDataFileName() const
{
  return "players/" + getUUIDasString() + ".json";
}

void Player::loadFromFile()
try
{
  if (!fs::exists(getPlayerDataFileName()))
  {
    mPosition.y = 50;
    return;
  }
  
  nlohmann::json j = nlohmann::json::parse(util::readFileToString(getPlayerDataFileName()));
  
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
}
catch (std::exception&) {}

void Player::saveToFile()
{
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

bool operator==(const Player& l, const Player& r)
{
  return &l == &r;
}

bool operator!=(const Player& l, const Player& r)
{
  return !(l == r);
}

void Player::onEntityMovedWithLook(PlayerPosition newpos)
{
  using PlayerPtrVector = std::vector<Player*>;
  PlayerPtrVector create;
  PlayerPtrVector update;
  PlayerPtrVector destroy;
  PlayerPtrVector nextEntitiesInSight;
  
  for (PlayerSharedPtr& player : mServer->getOnlinePlayers())
  {
    if (*player != *this)
    {
      auto distance = player->getPosition().distance(mPosition);
    
      if (distance <= InRange)
      {
        auto it = std::find(mEntitiesInSight.begin(), mEntitiesInSight.end(), player.get());
      
        if (it == mEntitiesInSight.end())
        {
          create.push_back(player.get());
          nextEntitiesInSight.push_back(player.get());
        }
        else
        {
          update.push_back(player.get());
          nextEntitiesInSight.push_back(player.get());
        }
      }
      else
      {
        destroy.push_back(player.get());
      }
    }
  }
  
  if (create.size() > 0)
  {
    packets::SpawnPlayer packet(*this);
    
    for (Player* player : create)
    {
      packet.send(*player);
    }
  }
  
  if (update.size() > 0)
  {
    std::int16_t dx = (util::floorAndCast(newpos.x * 32.0) - util::floorAndCast(mPosition.x * 32.0)) * 128;
    std::int16_t dy = (util::floorAndCast(newpos.y * 32.0) - util::floorAndCast(mPosition.y * 32.0)) * 128;
    std::int16_t dz = (util::floorAndCast(newpos.z * 32.0) - util::floorAndCast(mPosition.z * 32.0)) * 128;

    packets::EntityLookAndRelativeMove packet(mEntityID, dx, dy, dz, mPosition.yaw, mPosition.pitch, mPosition.onGround);

    for (Player* player : update)
    {
      packet.send(*player);
    }
  }
  
  mEntitiesInSight = nextEntitiesInSight;
}

void Player::updateChunks(const boost::system::error_code& error)
{
  using namespace std::chrono_literals;
  
  static constexpr std::int32_t Range = 5;
  
  if (!error && !mSession->isDisconnecting())
  {
    ChunkManager& cm = mWorld->getChunkManager();
    
    std::list<Vector2i> current;
    
    Vector2i playerchunk(static_cast<std::int32_t>(mPosition.x / 16), static_cast<std::int32_t>(mPosition.z / 16));
    
    Vector2i start(playerchunk.x - Range, playerchunk.z - Range - 1);
    Vector2i end(playerchunk.x + Range + 1, playerchunk.z + Range);
    
    for (std::int32_t x = start.x; x < end.x; ++x)
    {
      for (std::int32_t z = start.z; z < end.z; ++z)
      {
        Vector2i th(x, z);
        current.push_back(th);
        
        auto it = mChunksInUse.end();
        for (auto i = mChunksInUse.begin(); i != it; ++i)
        {
          if (th == *i)
          {
            it = i;
            break;
          }
        }
        
        if (it == mChunksInUse.end())
        {
          packets::ChunkData(cm.getChunk(th), th).send(*mSession);
        }
        else
        {
          mChunksInUse.erase(it);
        }
      }
    }
    
    for (auto& i : mChunksInUse)
    {
      packets::UnloadChunk(i).send(*mSession);
    }
    
    mChunksInUse = std::move(current);
    
    mUpdateChunksTimer.expires_from_now(500ms);
    updateChunksNext();
  }
}

void Player::updateChunksNext()
{
  mUpdateChunksTimer.async_wait(boost::bind(&Player::updateChunks, shared_from_this(), asio::placeholders::error));
}

void Player::timersNext()
{
  keepAliveNext();
  updateChunksNext();
}

} // namespace red
