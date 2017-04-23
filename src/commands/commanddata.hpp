#ifndef REDI_COMMANDS_COMMANDDATA_HPP
#define REDI_COMMANDS_COMMANDDATA_HPP

#include <string>
#include <vector>
#include "command.hpp"

namespace redi {
namespace commands {

class Command;

struct CommandData {
  Command* ptr;
  std::string command;
  std::vector<std::string> aliases;
};

inline bool operator==(const CommandData& data, const std::string& str) {
  if (data.command == str) {
    return true;
  }
  
  return std::find(data.aliases.begin(), data.aliases.end(), str) != data.aliases.end();
}

}
}

#endif // REDI_COMMANDS_COMMANDDATA_HPP