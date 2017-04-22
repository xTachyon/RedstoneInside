#include <boost/format.hpp>
#include <boost/program_options/parsers.hpp>
#include "commandmanager.hpp"
#include "command.hpp"
#include "../util/util.hpp"
#include "../logger.hpp"

namespace po = boost::program_options;

namespace redi {
namespace commands {

CommandAddResult
CommandManager::registerCommand(Command* ptr, string_view command, const std::vector<string_view>& aliases) {
  if (!ptr) {
    return CommandAddResult::NullPointer;
  }
  
  {
    bool b = commands.count(command) == 0;
    
    for (const auto& i : aliases) {
      if (!b) {
        break;
      }
      b = commands.count(i) == 0;
    }
    
    if (!b) {
      return CommandAddResult::AlreadyExists;
    }
  }
  
  data[ptr].push_back({});
  CommandData& cd = data[ptr].back();
  cd.ptr = ptr;
  cd.command = command;
  cd.aliases = aliases;
  
  commands[command] = &cd;
  for (const auto& i : aliases) {
    commands[i] = &cd;
  }
  
  return CommandAddResult::Ok;
}

CommandManager& CommandManager::operator()(CommandSender& sender, string_view message) {
  using namespace std::string_literals;
  
  Logger::info((boost::format("%1% issued server command \"%2%\"") %
                sender.getSenderName() % message));
  
  message.remove_prefix(1);
  // TODO: come back here
  std::string s(message);
  std::vector<std::string> splited = po::split_unix(s);
  
  if (splited.size() == 0) {
    sender.sendMessageToSender("Command cannot be empty");
  }
  else {
    splited[0] = util::toLowercase(splited[0]);
  }
  auto it = commands.find(splited[0]);
  if (it == commands.end()) {
    sender.sendMessageToSender(splited[0] + " does not exist");
  }
  else {
    assert(it->second->ptr && "The pointer shouldn't be null");
    Command& ref = *it->second->ptr;
    
    // TODO: find a way which doesn't involve copying
    CommandArguments args;
    args.reserve(splited.size() - 1);
    
    for (std::size_t i = 1; i < splited.size(); ++i) {
      args.emplace_back(splited[i]);
    }
    
    ref(sender, it->second->command, args);
  }
  
  return *this;
}

void CommandManager::unregisterAll(Command* ptr) {
  auto& vec = data[ptr];
  for (const auto& i : vec) {
    commands.erase(i.command);
    
    for (const auto& j : i.aliases) {
      commands.erase(j);
    }
  }
  
  data.erase(ptr);
}

}
} // namespace redi