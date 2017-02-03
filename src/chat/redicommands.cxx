#include "../player.hpp"
#include "commandmanager.hpp"
#include "redicommands.hpp"
#include "../exceptions.hpp"

namespace redi
{

RediCommands::RediCommands(CommandManager& manager) : mManager(manager)
{
  CommandData data;
  
  data.command = "position";
  data.callback = positionCommand;
  mIterators.push_back(mManager.registerCommand(std::move(data)));
  
  data.command = "pos";
  data.callback = positionCommand;
  mIterators.push_back(mManager.registerCommand(std::move(data)));
  
  data.command = "uuid";
  data.callback = uuidCommand;
  mIterators.push_back(mManager.registerCommand(std::move(data)));
  
  data.command = "stop";
  data.callback = stopCommand;
  mIterators.push_back(mManager.registerCommand(std::move(data)));
}

RediCommands::~RediCommands()
{
  for (const auto& it : mIterators)
  {
    mManager.unregisterCommand(it);
  }
}

void RediCommands::positionCommand(CommandSender sender, CommandArguments& args)
{
  if (sender.isNotPlayer()) return;
  Player& player = sender.getPlayer();
  
  player.sendMessage((boost::format("%1%") % static_cast<Vector3d>(player.getPosition())).str());
  
  static_cast<void>(args);
}

void RediCommands::uuidCommand(CommandSender sender, CommandArguments& args)
{
  if (sender.isNotPlayer()) return;
  Player& player = sender.getPlayer();
  
  player.sendMessage(player.getUUIDasString());
  
  static_cast<void>(args);
}

void RediCommands::stopCommand(CommandSender sender, CommandArguments& args)
{
  throw StopServer();
  
  static_cast<void>(args);
}
  
} // namespace redi