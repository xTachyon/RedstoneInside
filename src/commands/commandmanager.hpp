#ifndef REDI_COMMANDS_COMMANDMANAGER_HPP
#define REDI_COMMANDS_COMMANDMANAGER_HPP

#include <unordered_map>
#include "../hasserver.hpp"
#include "commandsender.hpp"
#include "../datatypes.hpp"
#include "commanddata.hpp"

namespace redi {
namespace commands {

class Command;

enum class CommandAddResult {
  AlreadyExists, NullPointer, Ok
};

class CommandManager : HasServer {
public:
  
  using CommandsContainer = std::unordered_map<string_view, CommandData>;
  
  CommandManager(Server& server) : HasServer(server) {}
  
  CommandManager& operator()(CommandSender& sender, string_view message);
  
  CommandAddResult registerCommand(Command* ptr, string_view command, const std::vector<string_view>& aliases = {});
  
  // void unregisterCommand(string_view command);
  void unregisterAll(Command* ptr);
private:
  std::unordered_map<string_view, CommandData*> commands;
  std::unordered_map<Command*, std::list<CommandData>> data;
};

}
} // namespace redi

#endif // REDI_COMMANDS_COMMANDMANAGER_HPP