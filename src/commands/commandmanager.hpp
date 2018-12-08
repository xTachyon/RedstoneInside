#pragma once

#include <list>
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
  
  using CommandsContainer = std::unordered_map<std::string, CommandData>;
  
  CommandManager(Server& server) : HasServer(server) {}
  
  CommandManager& operator()(CommandSender& sender, std::string& message);
  
  CommandAddResult
  registerCommand(Command* ptr, const std::string& command, const std::vector<std::string>& aliases = {});
  
  // void unregisterCommand(string_view command);
  void unregisterAll(Command* ptr);
private:
  std::unordered_map<std::string, CommandData*> commands;
  std::unordered_map<Command*, std::list<CommandData>> data;
};

}
} // namespace redi