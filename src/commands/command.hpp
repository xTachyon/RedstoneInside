#ifndef REDI_COMMANDS_COMMAND_HPP
#define REDI_COMMANDS_COMMAND_HPP

#include "../hasserver.hpp"
#include "commandmanager.hpp"
#include "commandsender.hpp"

namespace redi {
namespace commands {

using CommandArguments = std::vector<string_view>;

class CommandManager;

class Command : HasServer {
public:
  Command(Server& server);
  
  virtual ~Command();
  
  virtual Command& operator()(CommandSender& sender, string_view command,
                              CommandArguments& args);

protected:
  CommandManager& manager;
};

}
} // namespace redi

#endif // REDI_COMMANDS_COMMAND_HPP