#include <boost/program_options/parsers.hpp>
#include "commandmanager.hpp"
#include "command.hpp"
#include "../util/util.hpp"
#include "../logger.hpp"

namespace po = boost::program_options;

namespace redi {
namespace commands {

CommandAddResult CommandManager::registerCommand(std::string&& command, Command* ptr) {
  ownership.emplace_front(std::move(command));
  return registerCommand(command, ptr, ownership.begin());
}

CommandAddResult CommandManager::registerCommand(const std::string& command, Command* ptr) {
  ownership.emplace_front(command);
  return registerCommand(command, ptr, ownership.begin());
}

CommandAddResult CommandManager::registerCommand(string_view command, Command* ptr) {
  return registerCommand(command, ptr, CommandManager::OwnershipListConstIt());
}

void CommandManager::unregisterCommand(string_view command) {
  auto it = commandsdata.find(command);
  assert(it != commandsdata.end() && "The iterator should always exist.");
  freeownership(it->second.it);
  commandsdata.erase(it);
}

CommandManager& CommandManager::operator()(CommandSender& sender, string_view message) {
  using namespace std::string_literals;
  
  Logger::info((boost::format("%1% issued server command \"%2%\"") %
                sender.getSenderName() % message));
  
  message.remove_prefix(1);
  std::vector<std::string> splited = po::split_unix(message.to_string());
  
  if (splited.size() == 0) {
    sender.sendMessageToSender("Command cannot be empty");
  }
  else {
    splited[0] = util::toLowercase(splited[0]);
    string_view command = splited[0];
    
    auto it = commandsdata.find(command);
    if (it == commandsdata.end()) {
      sender.sendMessageToSender(command.to_string() + " does not exist");
    }
    else {
      assert(it->second.ptr && "The pointer shouldn't be null");
      Command& ref = *it->second.ptr;
      
      // TODO: find a way which doesn't involve copying
      CommandArguments args;
      args.reserve(splited.size() - 1);
      
      for (std::size_t i = 1; i < splited.size(); ++i) {
        args.emplace_back(splited[i]);
      }
      
      ref(sender, command, args);
    }
  }
  
  return *this;
}

CommandAddResult
CommandManager::registerCommand(string_view command, Command* ptr, CommandManager::OwnershipListConstIt it) {
  
  if (!ptr) {
    freeownership(it);
    return CommandAddResult::NullPointer;
  }
  if (commandsdata.count(command) > 0) {
    freeownership(it);
    return CommandAddResult::AlreadyExists;
  }
  
  auto& cd = commandsdata[command];
  cd.ptr = ptr;
  cd.it = it;
  
  return CommandAddResult::Ok;
}

void CommandManager::freeownership(CommandManager::OwnershipListConstIt it) {
  if (it != ownership.end()) {
    ownership.erase(it);
  }
}

void CommandManager::unregisterAll(Command* ptr) {
  auto it = commandsdata.begin();
  while (it != commandsdata.end()) {
    if (it->second.ptr == ptr) {
      it = commandsdata.erase(it);
    }
    else {
      ++it;
    }
  }
}

}
} // namespace redi