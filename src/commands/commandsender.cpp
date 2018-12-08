#include <stdexcept>
#include "commandsender.hpp"

namespace redi {
namespace commands {

Player& CommandSender::getPlayer() {
  throw std::runtime_error("NNOOOOOOOOOOOOOOOOOOOOOOPE");
}

Server& CommandSender::getSenderServer() {
  throw std::runtime_error("NNOOOOOOOOOOOOOOOOOOOOOOPE");
}

void CommandSender::sendMessageToSender(const std::string&) {
  throw std::runtime_error("NNOOOOOOOOOOOOOOOOOOOOOOPE");
}

const std::string& CommandSender::getSenderName() const {
  throw std::runtime_error("NNOOOOOOOOOOOOOOOOOOOOOOPE");
}

Player* CommandSender::getPlayerPtr() {
  return isPlayer() ? &getPlayer() : nullptr;
}

}
}