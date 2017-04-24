#include "command.hpp"
#include "../server.hpp"
#include "../player.hpp"

namespace redi {
namespace commands {

Command::Command(Server & server)
    : HasServer(server), manager(server.getCommandManager()) {}

Command& Command::operator()(CommandSender&, const std::string&,
                             CommandArguments&) {
  return *this;
}

Command::~Command() {
  manager.unregisterAll(this);
}

Player* Command::getPlayerOrDefault(CommandSender& sender, const CommandArguments& args) const {
  Player * player{};
  
  if (args.size() == 0) {
    player = sender.getPlayerPtr();
  }
  else {
    player = sender.getSenderServer().findPlayer(args[0]);
  }
  
  return player;
}

Player* Command::getPlayerOrDefaultAndSendMessageIfNot(CommandSender& sender, const CommandArguments& args) const {
  Player * player = getPlayerOrDefault(sender, args);
  if (!player) {
    sender.sendMessageToSender("Can't find the player");
  }
  return player;
}

}
} // namespace redi