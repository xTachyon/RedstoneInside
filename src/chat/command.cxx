#include "../server.hpp"
#include "command.hpp"

namespace redi {
namespace commands {

Command::Command(Server & server)
    : HasServer(server), manager(server.getCommandManager()) {}

Command& Command::operator()(CommandSender&, string_view,
                             CommandArguments&) {
  return *this;
}

Command::~Command() {
  manager.unregisterAll(this);
}

}
} // namespace redi