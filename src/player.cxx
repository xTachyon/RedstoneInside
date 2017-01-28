#include <chrono>
#include <boost/format.hpp>
#include "player.hpp"
#include "server.hpp"
#include "logger.hpp"

namespace asio = boost::asio;

namespace redi
{

Player::Player(const std::string& name, boost::uuids::uuid uuid, std::unique_ptr<Session>&& session, std::int32_t id, Server* server,
               World* world, Gamemode gamemode)
    : mUUID(uuid), mNickname(name), mServer(server), mWorld(world), mSession(std::move(session)), mGamemode(gamemode), mSendKeepAlive(mSession->getIoService()),
      mEntityID(id)
{
  Logger::info((boost::format("%1% has joined the game") % mNickname).str());

  mSendKeepAlive.expires_from_now(std::chrono::seconds(5));
  mSendKeepAlive.async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, asio::placeholders::error, std::addressof(mSendKeepAlive), &mSession->getProtocol()));
}

Player::~Player()
{
  Logger::debug((boost::format("Player %1% destroyed") % this).str());
  Logger::info((boost::format("%1% has left the game") % mNickname).str());
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
  ByteBuffer buf(mSession->getProtocol().createChatPacket(json, position));
  ByteBufferSharedPtr ptr(std::make_shared<ByteBuffer>(std::move(buf)));
  
  sendPacket(ptr);
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
