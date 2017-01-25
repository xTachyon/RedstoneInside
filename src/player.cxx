#include <boost/format.hpp>
#include "player.hpp"
#include "server.hpp"
#include "logger.hpp"

namespace redi
{

Player::Player(const std::string& name, const std::string uuid, Session* session, std::int32_t id, Server* server, World* world,
               Gamemode gamemode)
  : id(id), mNickname(name), mUUID(uuid), mSession(session), mServer(server), mGamemode(gamemode), mSendKeepAlive(session->getIoService()), mDimension(Dimension::Overworld),
    mWorld(world)
{
  Logger::info((boost::format("%1% has joined the game") % mNickname).str());
  
  mSendKeepAlive.expires_from_now(boost::posix_time::seconds(5));
  mSendKeepAlive.async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, boost::asio::placeholders::error, std::addressof(mSendKeepAlive), &mSession->getProtocol()));
}

Player::~Player()
{
  Logger::debug((boost::format("Player %1% destroyed") % this).str());
  Logger::info((boost::format("%1% has left the game") % mNickname).str());
}

void Player::onSendKeepAliveTimerRing(const boost::system::error_code& error, boost::asio::deadline_timer* timer,
                                     Protocol* protocol)
{
  if (!error)
  {
    protocol->sendKeepAlive();
    
    timer->expires_from_now(boost::posix_time::seconds(10));
    timer->async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, boost::asio::placeholders::error, timer, protocol));
  }
  
  Logger::debug("Send Keep Alive Timer Ring " + std::to_string(error.value()));
}

void Player::sendPacket(ByteBufferSharedPtr ptr)
{
  mSession->sendPacket(ptr);
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