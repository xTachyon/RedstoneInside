#include <functional>
#include "../server.hpp"
#include "commandmanager.hpp"
#include "redicommands.hpp"
#include "../exceptions.hpp"
#include "../player.hpp"
#include "../util/util.hpp"

namespace redi
{

RediCommands::RediCommands(CommandManager& manager) : mCommandManager(manager)
{
  CommandData data;
  
  data.command = "position";
  data.callback = positionCommand;
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
  
  data.command = "pos";
  data.callback = positionCommand;
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
  
  data.command = "rotation";
  data.callback = rotationCommand;
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
  
  data.command = "rot";
  data.callback = rotationCommand;
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
  
  data.command = "uuid";
  data.callback = uuidCommand;
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
  
  data.command = "stop";
  data.callback = std::bind(&RediCommands::stopCommand, this, std::placeholders::_1, std::placeholders::_2);
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
  
  data.command = "w";
  data.callback = std::bind(&RediCommands::whisperCommand, this, std::placeholders::_1, std::placeholders::_2);
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
  
  data.command = "kick";
  data.callback = std::bind(&RediCommands::kickCommand, this, std::placeholders::_1, std::placeholders::_2);
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
  
  data.command = "doesntwork";
  data.callback = doesntWork;
  mIterators.push_back(mCommandManager.registerCommand(std::move(data)));
}

RediCommands::~RediCommands()
{
  for (const auto& it : mIterators)
  {
    mCommandManager.unregisterCommand(it);
  }
}

void RediCommands::positionCommand(CommandSender sender, CommandArguments&)
{
  if (sender.isNotPlayer()) return;
  Player& player = sender.getPlayer();
  
  player.sendMessage((boost::format("Position: %1%") % static_cast<Vector3d>(player.getPosition())).str());
}

void RediCommands::rotationCommand(CommandSender sender, CommandArguments&)
{
  if (sender.isNotPlayer()) return;
  Player& player = sender.getPlayer();
  
  player.sendMessage((boost::format("Rotation: %1%") % static_cast<redi::PlayerLook>(player.getPosition())).str());
}

void RediCommands::uuidCommand(CommandSender sender, CommandArguments&)
{
  if (sender.isNotPlayer()) return;
  Player& player = sender.getPlayer();
  
  player.sendMessage("UUID: " + player.getUUIDasString());
}

void RediCommands::stopCommand(CommandSender, CommandArguments&)
{
  
  
  throw StopServer();
}

void RediCommands::whisperCommand(CommandSender sender, CommandArguments& args)
{
  if (args.size() == 0)
  {
    sender.sendMessage("Usage: /w [player] <message>");
    return;
  }
  
  Player* player = mCommandManager.getServer().findPlayer(args[0]);
  if (player)
  {
    std::string message;
    for (CommandArguments::const_iterator it = args.begin() + 1, end; it != end; ++it)
    {
      message += *it + ' ';
    }
    message.pop_back();
    
    player->sendMessage((boost::format("%1% -> %2%: %3%") % sender.getName() % player->getUsername() % message).str());
  }
  else
  {
    sender.sendMessage("No player named " + args[0]);
  }
}

void RediCommands::kickCommand(CommandSender sender, CommandArguments& args)
{
  if (args.size() == 0)
  {
    sender.sendMessage("Usage: /kick [player] <message>");
    return;
  }
  
  auto getMessage = [&]()
  {
    std::string message;
    
    for (auto it = args.begin() + 1; it != args.end(); ++it)
    {
      message += *it + ' ';
    }
    
    return message;
  };
  
  if (sender.isPlayer() && (util::noCaseCompareEqual(sender.getName(), args[0]) || args[0] == "myself"))
  {
    sender.getPlayer().kick(getMessage());
  }
  
  Player* player = mCommandManager.getServer().findPlayer(args[0]);
  
  if (player)
  {
    player->kick(getMessage());
  }
  else
  {
    sender.sendMessage((boost::format("No player named \"%1%\"") % args[0]).str());
  }
}

void RediCommands::doesntWork(CommandSender sender, CommandArguments& args)
{
  std::string message;
  
  for (const std::string& index : args)
  {
    message += index + ' ';
  }
  
  if (args.size() != 0)
  {
    message.pop_back();
    message += ": ";
  }
  message += "Look buddy, doesn't work is a vague statement. Does it sit on the couch all day long? "
        "Does it procrastinate doing the dishes? Does it beg on the street for change? Please be specific! "
        "Define 'it' and what it isn't doing. Give us more details so we can help you without needing to ask basic questions like what's the error message?";
  
  sender.getServer().getChatManager()(message);
}
  
} // namespace redi