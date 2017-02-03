#include <boost/program_options/parsers.hpp>
#include "commandmanager.hpp"

namespace po = boost::program_options;

namespace redi
{

CommandManager::CommandManager(Server& server) : mServer(server) {}

void CommandManager::operator()(Player& player, std::string&& command)
{
  command.erase(command.begin());
  
  std::vector<std::string> splited = po::split_unix(command);
  command = splited[0];
  splited.erase(splited.begin());
  
  for (auto& index : mCommands)
  {
    if (index.command == command) index.callback(CommandSender(player), index.command, command, splited);
    
    for (auto idx : index.aliases)
    {
      if (idx == command) index.callback(CommandSender(player), index.command, command, splited);
    }
  }
}

CommandManager::CommandListIterator CommandManager::registerCommand(const CommandData& command)
{
  return registerCommand(CommandData(command));
}

CommandManager::CommandListIterator CommandManager::registerCommand(CommandData&& command)
{
  if (commandExistsOrAlias(command)) throw std::runtime_error("The command or one of it's aliases already exists");
  
  mCommands.emplace_back(std::move(command));
  return mCommands.end();
}

void CommandManager::unregisterCommand(const std::string& command)
{
  mCommands.remove_if([&](const CommandData& data)
                      {
                        if (data.command == command) return true;
                        
                        for (const auto& index : data.aliases)
                        {
                          if (index == command) return true;
                        }
                        
                        return false;
                      });
}

void CommandManager::unregisterCommand(CommandListIterator it)
{
  mCommands.erase(it);
}

bool CommandManager::commandExistsOrAlias(const std::string& command)
{
  for (const auto& index : mCommands)
  {
    if (index.command == command) return true;
    
    for (const auto& idx : index.aliases)
    {
      if (idx == command) return true;
    }
  }
  
  return false;
}

bool CommandManager::commandExistsOrAlias(const CommandData& command)
{
  if (commandExistsOrAlias(command.command)) return true;
  
  for (const auto& index : command.aliases)
  {
    if (commandExistsOrAlias(index)) return true;
  }
  
  return false;
}
  
} // namespace redi