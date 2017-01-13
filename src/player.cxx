#include <boost/format.hpp>
#include "player.hpp"
#include "server.hpp"
#include "logger.hpp"

namespace redi
{

Player::Player(const std::string& name, const std::string uuid, Session* session, std::int32_t id, Server* server,
               Gamemode gamemode)
  : id(id), mNickname(name), mUUID(uuid), mSession(session), mServer(server), mGamemode(gamemode), mSendKeepAlive(session->getIoService()) //, mReceivedKeepAlive(session->getIoService())
{
  mSendKeepAlive.expires_from_now(boost::posix_time::seconds(1));
  mSendKeepAlive.async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, boost::asio::placeholders::error, std::addressof(mSendKeepAlive), mSession->getProtocolPtr()));
}

Player::~Player()
{
  Logger::info((boost::format("Player %1% destroyed") % this).str());
}

void Player::onSendKeepAliveTimerRing(const boost::system::error_code& error, boost::asio::deadline_timer* timer,
                                     Protocol* protocol)
{
  if (!error)
  {
    protocol->sendKeepAkive();
    
    timer->expires_from_now(boost::posix_time::seconds(10));
    timer->async_wait(boost::bind(&Player::onSendKeepAliveTimerRing, boost::asio::placeholders::error, timer, protocol));
  }
  
  Logger::info("Send Keep Alive Timer Ring " + std::to_string(error.value()));
}
  
} // namespace red