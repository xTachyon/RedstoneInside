#ifndef REDI_COMMANDSENDER_HPP
#define REDI_COMMANDSENDER_HPP

//#include "../server.hpp"

namespace redi {

class Player;

class Server;

enum class CommandSenderType { Player, Server };

class CommandSender {
public:
  CommandSender(CommandSenderType type)
      : type(type) {}
  
  // virtual ~CommandSender() = 0;
  
  virtual Player& getPlayer();
  
  virtual Server& getSenderServer();
  
  bool isPlayer() const { return type == CommandSenderType::Player; }

private:
  CommandSenderType type;
};

}

#endif // REDI_COMMANDSENDER_HPP