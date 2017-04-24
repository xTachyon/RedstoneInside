#include "../server.hpp"
#include "../exceptions.hpp"
#include "redicommands.hpp"

namespace redi {
namespace commands {

RediCommands::RediCommands(Server & server)
    : Command(server) {
  manager.registerCommand(this, "stop");
  manager.registerCommand(this, "position", {"pos"});
  manager.registerCommand(this, "rotation", {"rot"});
}

Command& RediCommands::operator()(CommandSender& sender, const std::string& command, CommandArguments& args) {
  
  if (command == "stop") {
    stop(sender);
  }
  else if (command == "position") {
    position(sender, args);
  }
  else if (command == "rotation") {
    rotation(sender);
  }
  
  return *this;
}

void RediCommands::stop(CommandSender& sender) {
  sender.getSenderServer().stop();
}

void RediCommands::position(CommandSender& sender, CommandArguments& args) {
  Player * target = nullptr;
  if (args.size() == 0) {
    target = &sender.getPlayer();
  }
  else {
    // TODO: come back here
    std::string s(args[0]);
    target = sender.getSenderServer().findPlayer(s);
  }
  
  if (target) {
    sender.sendMessageToSender(target->getPosition().toString());
  }
  else {
    sender.sendMessageToSender("Cannot find the player");
  }
}

void RediCommands::rotation(CommandSender& sender) {
  if (!checkIfPlayer(sender)) {
    return;
  }
  
  sender.sendMessageToSender((boost::format("Rotation: %1%") %
                              static_cast<redi::PlayerLook>(sender.getPlayer().getPosition()))
                                 .str());
}

void RediCommands::uuid(CommandSender& sender, CommandArguments& args) {

}

void RediCommands::whisper(CommandSender& sender, CommandArguments& args) {

}

void RediCommands::kick(CommandSender& sender, CommandArguments& args) {

}

void RediCommands::doesntwork(CommandSender& sender, CommandArguments& args) {

}

bool RediCommands::checkIfPlayer(CommandSender& sender) {
  if (sender.isPlayer()) {
    return true;
  }
  
  sender.sendMessageToSender("This command can only be used by player");
  return false;
}

}
} // namespace redi