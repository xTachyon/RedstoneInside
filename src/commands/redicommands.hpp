#ifndef REDI_CHAT_REDICOMMANDS_HPP
#define REDI_CHAT_REDICOMMANDS_HPP

#include "command.hpp"

namespace redi {
namespace commands {

class RediCommands : public Command {
public:
  RediCommands(Server& server);
  
  Command& operator()(CommandSender& sender, string_view command,
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

#endif // REDI_CHAT_REDICOMMANDS_HPP