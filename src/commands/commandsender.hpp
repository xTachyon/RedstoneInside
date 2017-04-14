#ifndef REDI_COMMANDSENDER_HPP
#define REDI_COMMANDSENDER_HPP

#include "../datatypes.hpp"

namespace redi {

class Player;

class Server;

namespace commands {

enum class CommandSenderType { Player, Server };

class CommandSender {
public:
  CommandSender(CommandSenderType type)
      : type(type) {}
  
  // virtual ~CommandSender() = 0;
  /*
   * Don't destroy it through this destructor, please ?
   */
  
  virtual Player& getPlayer();
  
  virtual Server& getSenderServer();
  
  virtual void sendMessageToSender(string_view message);
  
  virtual string_view getSenderName() const;
  
  bool isPlayer() const { return type == CommandSenderType::Player; }
  
  bool isNotPlayer() const { return !isPlayer(); }

private:
  CommandSenderType type;
};

}
}

#endif // REDI_COMMANDSENDER_HPP