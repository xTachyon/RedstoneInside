#ifndef REDI_CHAT_REDICOMMANDS_HPP
#define REDI_CHAT_REDICOMMANDS_HPP

#include <vector>
#include "commandmanager.hpp"
#include "commandsender.hpp"

namespace redi
{

class CommandManager;

class RediCommands
{
public:
  
  RediCommands(CommandManager& manager);
  ~RediCommands();
  
  static void positionCommand(CommandSender sender, CommandArguments& args);
  static void uuidCommand(CommandSender sender, CommandArguments& args);
  static void stopCommand(CommandSender sender, CommandArguments& args);
  void whisperCommand(CommandSender sender, CommandArguments& args);
  void kickCommand(CommandSender sender, CommandArguments& args);
  
private:
  
  CommandManager& mCommandManager;
  std::vector<CommandListIterator> mIterators;
};

} // namespace redi

#endif // REDI_CHAT_REDICOMMANDS_HPP