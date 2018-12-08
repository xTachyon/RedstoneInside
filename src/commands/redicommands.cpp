#include "../server.hpp"
#include "redicommands.hpp"

namespace redi {
namespace commands {

RediCommands::RediCommands(Server & server)
    : Command(server) {
  manager.registerCommand(this, "stop");
  manager.registerCommand(this, "position", {"pos"});
  manager.registerCommand(this, "rotation", {"rot"});
  manager.registerCommand(this, "uuid");
  manager.registerCommand(this, "whisper", {"w"});
  manager.registerCommand(this, "kick");
  manager.registerCommand(this, "doesntwork");
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
  else if (command == "uuid") {
    uuid(sender, args);
  }
  else if (command == "whisper") {
    whisper(sender, args);
  }
  else if (command == "kick") {
    kick(sender, args);
  }
  else if (command == "doesntwork") {
    doesntwork(sender, args);
  }
  
  return *this;
}

void RediCommands::stop(CommandSender& sender) {
  sender.getSenderServer().stop();
}

void RediCommands::position(CommandSender& sender, CommandArguments& args) {
  Player* target = nullptr;
  if (args.size() == 0) {
    target = &sender.getPlayer();
  }
  else {
    target = sender.getSenderServer().findPlayer(args[0]);
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
  auto* target = getPlayerOrDefaultAndSendMessageIfNot(sender, args);
  if (target) {
    sender.sendMessageToSender(target->getUUIDasString());
  }
}

void RediCommands::whisper(CommandSender& sender, CommandArguments& args) {
  if (args.size() < 2) {
    sender.sendMessageToSender("Player or message empty");
  }
  else {
    auto* player = sender.getSenderServer().findPlayer(args[0]);
    
    if (player) {
      
      std::string message = sender.getSenderName() + " -> " + player->getUsername() + ": ";
      auto end = args.end();
      for (auto it = args.begin(); it != end; ++it) {
        message += *it + ' ';
      }
      
      player->sendMessageToSender(message);
    }
    else {
      sender.sendMessageToSender("Can't find the player: " + args[0]);
    }
  }
}

void RediCommands::kick(CommandSender& sender, CommandArguments& args) {
  auto* target = getPlayerOrDefaultAndSendMessageIfNot(sender, args);
  if (target) {
    target->kick();
  }
}

void RediCommands::doesntwork(CommandSender& sender, CommandArguments& args) {
  using namespace std::string_literals;
  
  std::string message;
  if (args.size() != 0) {
    message += args[0] + ": ";
  }
  
  message += "Look buddy, doesn't work is a vague statement. Does it sit on "
      "the couch all day long? "
      "Does it procrastinate doing the dishes? Does it beg on the "
      "street for change? Please be specific! "
      "Define 'it' and what it isn't doing. Give us more details so we "
      "can help you without needing to ask basic questions like what's "
      "the error message?";
  
  sender.getSenderServer().getChatManager().broadcast(message);
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