#include <boost/program_options/parsers.hpp>
#include "commandmanager.hpp"
#include "../logger.hpp"

namespace po = boost::program_options;

namespace redi
{

CommandManager::CommandManager(Server& server) : mServer(server) {}

void CommandManager::operator()(Player& player, std::string& command)
{
  Logger::info((boost::format("%1% issued server command \"%2%\"") % player.getUsername() % command));
  
  command.erase(command.begin());
  
  std::vector<std::string> splited = po::split_unix(command);
  if (splited.size() == 0) return;
  command = splited[0];
  splited.erase(splited.begin());
  
  for (auto& index : mCommands)
  {
    if (index.command == command)
    {
      index.callback(CommandSender(player), splited);
    }
  }
}

CommandListIterator CommandManager::registerCommand(const CommandData& command)
{
  return registerCommand(CommandData(command));
}

CommandListIterator CommandManager::registerCommand(CommandData&& command)
{
  if (commandExists(command.command)) throw std::runtime_error("The command or one of it's aliases already exists");
  
  mCommands.emplace_front(std::move(command));
  return mCommands.begin();
}

void CommandManager::unregisterCommand(const std::string& command)
{
  mCommands.remove_if([&](const CommandData& data)
                      {
                        return data.command == command;
                      });
}

void CommandManager::unregisterCommand(CommandListIterator it)
{
  mCommands.erase(it);
}

bool CommandManager::commandExists(const std::string& command)
{
  for (const auto& index : mCommands)
  {
    if (index.command == command)
    {
      return true;
    }
  }
  
  return false;
}

} // namespace redi