#include "../server.hpp"
#include "commandsender.hpp"

namespace redi {

std::string CommandSender::getName() const {
  return isPlayer() ? getPlayer().getUsername() : "CONSOLE";
}

void CommandSender::sendMessage(const std::string& message) {
  if (isPlayer())
    getPlayer().sendMessage(message);
  else
    getServer().sendMessage(message);
}

} // namespace redi