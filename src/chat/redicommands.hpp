#ifndef REDI_CHAT_REDICOMMANDS_HPP
#define REDI_CHAT_REDICOMMANDS_HPP

#include "command.hpp"

namespace redi {
namespace commands {

class RediCommands : Command {
public:
  RediCommands(Server& server);
  
  Command& operator()(CommandSender& sender, string_view command,
                      CommandArguments& args) override;
  
  void stopCommand(CommandSender& sender, CommandArguments& args);
};

}
} // namespace redi

#endif // REDI_CHAT_REDICOMMANDS_HPP