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

namespace asio = boost::asio;
namespace fs = boost::filesystem;

namespace redi
{

Player::Player(const std::string& name, boost::uuids::uuid uuid, std::shared_ptr<Session> session, std::int32_t id, Server* server,
               World* world, Gamemode gamemode)
    : mUUID(uuid), mNickname(name), mServer(server), mWorld(world), mSession(session), mGamemode(gamemode), mSendKeepAlive(mSession->getIoService()),
      mTeleportID(0), mEntityID(id)
{
  Logger::debug((boost::format("Player %1% created") % this).str());
  
//  mSendKeepAlive.expires_from_now(std::chrono::seconds(5));
//  mSendKeepAlive.async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, asio::placeholders::error, std::addressof(mSendKeepAlive), mSession));
  
  loadFromFile();
}

Player::~Player()
{
  Logger::debug((boost::format("Player %1% destroyed") % this).str());
  
  saveToFile();
}

void Player::onSendKeepAliveTimer(const boost::system::error_code& error)
{
  using namespace std::chrono_literals;
  
  if (!error && !mSession->isDisconnecting())
  {
    packets::KeepAlive(5).send(*mSession);
    
    mSendKeepAlive.expires_from_now(15s);
    keepAliveNext();
  }
}

void Player::keepAliveNext()
{
  mSendKeepAlive.async_wait(boost::bind(&Player::onSendKeepAliveTimer, shared_from_this(), asio::placeholders::error));
}

void Player::onSendKeepAliveTimerRing(const boost::system::error_code& error, boost::asio::steady_timer* timer, SessionSharedPtr session)
{
  if (!error)
  {
    packets::KeepAlive(3).send(*session);

    timer->expires_from_now(std::chrono::seconds(15));
    timer->async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, boost::asio::placeholders::error, timer, session));
  }
}

void Player::sendPacket(ByteBufferSharedPtr ptr)
{
  mSession->sendPacket(ptr);
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
  mSession->disconnect();
}

void Player::kick(std::string&& message)
{
  kickJSONmessage(ChatManager::componentToJson(ChatComponent(
        {
              ChatMessagePart(std::move(message))
        })));
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

} // namespace red
