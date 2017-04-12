#ifndef REDI_CHAT_REDICOMMANDS_HPP
#define REDI_CHAT_REDICOMMANDS_HPP

#include <vector>
#include "commandmanager.hpp"
#include "commandsender2.hpp"

namespace redi {

class CommandManager;

class RediCommands {
public:
  RediCommands(CommandManager& manager);
  ~RediCommands();
  
  static void positionCommand(CommandSenderOld sender, CommandArguments& args);
  
  static void rotationCommand(CommandSenderOld sender, CommandArguments& args);
  
  static void uuidCommand(CommandSenderOld sender, CommandArguments& args);
  
  static void doesntWork(CommandSenderOld sender, CommandArguments& args);
  
  void stopCommand(CommandSenderOld sender, CommandArguments& args);
  
  void whisperCommand(CommandSenderOld sender, CommandArguments& args);
  
  void kickCommand(CommandSenderOld sender, CommandArguments& args);

private:
  CommandManager& mCommandManager;
  std::vector<CommandListIterator> mIterators;
};

} // namespace redi

#endif // REDI_CHAT_REDICOMMANDS_HPP