#include "../server.hpp"
#include "command.hpp"

namespace redi {
namespace commands {

Command::Command(Server & server)
    : HasServer(server), manager(server.getCommandManager()) {}

Command& Command::operator()(CommandSender&, const std::string&,
                             CommandArguments&) {
  return *this;
}

Command::~Command() {
  manager.unregisterAll(this);
}

}
} // namespace redi