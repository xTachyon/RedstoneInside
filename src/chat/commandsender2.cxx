#include "../server.hpp"
#include "commandsender2.hpp"

namespace redi {

std::string CommandSenderOld::getName() const {
  return isPlayer() ? getPlayer().getUsername() : "CONSOLE";
}

void CommandSenderOld::sendMessage(const std::string& message) {
  if (isPlayer()) {
    getPlayer().sendMessage(message);
  }
  else {
    getServer().sendMessage(message);
  }
}

} // namespace redi