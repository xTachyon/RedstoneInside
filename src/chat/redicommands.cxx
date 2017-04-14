#include "../server.hpp"
#include "../exceptions.hpp"

namespace redi {
namespace commands {

RediCommands::RediCommands(Server & server)
    : Command(server) {
  manager.registerCommand(string_view("stop"), this);
}

Command& RediCommands::operator()(CommandSender& sender, string_view command, CommandArguments& args) {
  
  if (command == "stop") {
    stopCommand(sender, args);
  }
  
  return *this;
}

void RediCommands::stopCommand(CommandSender&, CommandArguments&) {
  throw StopServer();
}

}
} // namespace redi