#ifndef REDI_CHAT_COMMANDMANAGER_HPP
#define REDI_CHAT_COMMANDMANAGER_HPP

#include <functional>
#include <list>
#include "commandsender.hpp"

namespace redi
{

class Server;
struct CommandData;

using CommandArguments = std::vector<std::string>;
using CommandAliases = std::vector<std::string>;
using CommandCallback = std::function<void(CommandSender, CommandArguments&)>;
using CommandList = std::list<CommandData>;
using CommandListIterator = CommandList::const_iterator;

class CommandManager
{
public:
  
  
  
  CommandManager(Server& server);
  CommandManager(const CommandManager&) = delete;
  CommandManager(CommandManager&&) = delete;
  
  CommandManager& operator=(const CommandManager&) = delete;
  CommandManager& operator=(CommandManager&&) = delete;
  
  void operator()(Player& player, std::string& command);
  
  CommandListIterator registerCommand(const CommandData& command);
  CommandListIterator registerCommand(CommandData&& command);
  
  void unregisterCommand(const std::string& command);
  void unregisterCommand(CommandListIterator it);
  
  bool commandExists(const std::string& command);
  
private:
  
  Server& mServer;
  CommandList mCommands;
};

struct CommandData
{
  std::string command;
  CommandCallback callback;
  
  CommandData() = default;
  CommandData(const CommandData&) = default;
  CommandData(CommandData&&) = default;
  
  CommandData& operator=(const CommandData&) = default;
  CommandData& operator=(CommandData&&) = default;
};

} // namespace redi

#endif // REDI_CHAT_COMMANDMANAGER_HPP