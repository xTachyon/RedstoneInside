#ifndef REDI_COMMANDS_COMMANDMANAGER_HPP
#define REDI_COMMANDS_COMMANDMANAGER_HPP

#include <experimental/string_view>
#include <unordered_map>
#include "../HasServer.hpp"
#include "commandsender.hpp"
#include "../datatypes.hpp"

namespace redi {
namespace commands {

class Command;

using CommandArguments = std::vector<string_view>;

enum class CommandAddResult {
  AlreadyExists, NullPointer, Ok
};

class CommandManager : HasServer {
public:
  using OwnershipListConstIt = std::list<std::string>::const_iterator;
  
  struct CommandData {
    Command* ptr;
    OwnershipListConstIt it;
  };
  
  using CommandsContainer = std::unordered_map<string_view, CommandData>;
  
  CommandManager(Server& server) : HasServer(server) {}
  
  CommandManager& operator()(CommandSender& sender, string_view message);
  
  CommandAddResult registerCommand(std::string&& command, Command* ptr);
  
  CommandAddResult registerCommand(const std::string& command, Command* ptr);
  
  CommandAddResult registerCommand(string_view command, Command* ptr);
  
  void unregisterCommand(string_view command);
  
  void unregisterAll(Command* ptr);

private:
  CommandsContainer commandsdata;
  std::list<std::string> ownership;
  
  CommandAddResult registerCommand(string_view command, Command* ptr, OwnershipListConstIt it);
  
  void freeownership(OwnershipListConstIt it);
};

}
} // namespace redi

#endif // #ifndef REDI_COMMANDS_COMMANDMANAGER_HPP