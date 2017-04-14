#include "commandsender.hpp"

namespace redi {
namespace commands {

Player& CommandSender::getPlayer() {
  throw std::runtime_error("NNOOOOOOOOOOOOOOOOOOOOOOPE");
}

Server& CommandSender::getSenderServer() {
  throw std::runtime_error("NNOOOOOOOOOOOOOOOOOOOOOOPE");
}

void CommandSender::sendMessageToSender(string_view) {
  throw std::runtime_error("NNOOOOOOOOOOOOOOOOOOOOOOPE");
}

string_view CommandSender::getSenderName() const {
  throw std::runtime_error("NNOOOOOOOOOOOOOOOOOOOOOOPE");
}

}
}