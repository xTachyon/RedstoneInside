#include <boost/format.hpp>
#include "player.hpp"
#include "server.hpp"
#include "logger.hpp"

namespace redi
{

Player::Player(const std::string& name, const std::string uuid, Session* session, std::int32_t id, Server* server,
               Gamemode gamemode)
  : id(id), mNickname(name), mUUID(uuid), mSession(session), mServer(server), mGamemode(gamemode), mSendKeepAlive(session->getIoService())//, mReceivedKeepAlive(session->getIoService())
{
  onSendKeepAliveTimerRing();
}

Player::~Player()
{
  mSendKeepAlive.cancel();
  mServer->addEvent(EventPtr(new EventSessionDC(mSession)));
  Logger::info((boost::format("Player %1% destroyed") % this).str());
}

void Player::onSendKeepAliveTimerRing()
{
  mServer->addEvent(EventPtr(new EventSendKeepAliveRing(*mSession)));
  mSendKeepAlive.expires_from_now(boost::posix_time::seconds(15));
  mSendKeepAlive.async_wait(std::bind(&Player::onSendKeepAliveTimerRing, this));
}
  
} // namespace red