#pragma once

#include <string>
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

  Player* getPlayerPtr();
  virtual Player& getPlayer();
  virtual Server& getSenderServer();
  
  virtual void sendMessageToSender(const std::string& message);
  virtual const std::string& getSenderName() const;
  
  bool isPlayer() const { return type == CommandSenderType::Player; }
  bool isNotPlayer() const { return !isPlayer(); }

private:
  CommandSenderType type;
};

}
}