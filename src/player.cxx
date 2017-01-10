#include "player.hpp"

namespace redi
{


Player::Player(const std::string& name, const std::string uuid, SessionPtr session, std::int32_t id, Server* server,
               Gamemode gamemode)
  : id(id), mNickname(name), mUUID(uuid), mSession(session), mServer(server), mGamemode(gamemode) {}

} // namespace red