#include "player.hpp"

namespace redi
{


Player::Player(const std::string& name, const std::string uuid, SessionPtr session)
  : mNickname(name), mUUID(uuid), mSession(session) {}

} // namespace red