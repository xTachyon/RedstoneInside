#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "player.hpp"
#include "server.hpp"
#include "logger.hpp"
#include "util/util.hpp"

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
  
  mSendKeepAlive.expires_from_now(std::chrono::seconds(5));
//  mSendKeepAlive.async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, asio::placeholders::error, std::addressof(mSendKeepAlive), &mSession->getProtocol()));
  
  loadFromFile();
}

Player::~Player()
{
  Logger::debug((boost::format("Player %1% destroyed") % this).str());
  
  saveToFile();
}

void Player::onSendKeepAliveTimerRing(const boost::system::error_code& error, boost::asio::steady_timer* timer,
                                     Protocol* protocol)
{
  if (!error)
  {
    protocol->sendKeepAlive();

    timer->expires_from_now(std::chrono::seconds(10));
    timer->async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, boost::asio::placeholders::error, timer, protocol));
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
//  ByteBuffer buf(mSession->getProtocol().createChatPacket(json, position));
//  ByteBufferSharedPtr ptr(std::make_shared<ByteBuffer>(std::move(buf)));
  
//  sendPacket(ptr);
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

} // namespace red
