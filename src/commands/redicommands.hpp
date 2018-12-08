#pragma once

#include "command.hpp"

namespace redi {
namespace commands {

class RediCommands : public Command {
public:
  RediCommands(Server& server);
  
  Command& operator()(CommandSender& sender, const std::string& command,
                      CommandArguments& args) override;
  
  void stop(CommandSender& sender);
  void position(CommandSender& sender, CommandArguments& args);
  void rotation(CommandSender& sender);
  void uuid(CommandSender& sender, CommandArguments& args);
  void whisper(CommandSender& sender, CommandArguments& args);
  void kick(CommandSender& sender, CommandArguments& args);
  void doesntwork(CommandSender& sender, CommandArguments& args);
  bool checkIfPlayer(CommandSender& sender);
};

}
} // namespace redi