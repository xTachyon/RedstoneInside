#ifndef REDI_CHAT_COMMANDSENDER2_HPP
#define REDI_CHAT_COMMANDSENDER2_HPP

#include "../player.hpp"

namespace redi {

enum class CommandSenderType2 {
  Player, Server
};

class CommandSenderOld {
public:
  CommandSenderOld(Player& player)
      : mPtr(&player), mType(CommandSenderType2::Player) {}
  
  CommandSenderOld(Server& server)
      : mPtr(&server), mType(CommandSenderType2::Server) {}
  
  Player& getPlayer() { return get<Player, CommandSenderType2::Player>(); }
  
  const Player& getPlayer() const {
    return get<Player, CommandSenderType2::Player>();
  }
  
  Server& getServer() {
    if (isServer()) {
      return get<Server, CommandSenderType2::Server>();
    }
    return getPlayer().getServer();
  }
  
  const Server& getServer() const {
    if (isServer()) {
      return get<Server, CommandSenderType2::Server>();
    }
    return getPlayer().getServer();
  }
  
  CommandSenderType2 getType() const { return mType; }
  
  bool isPlayer() const { return mType == CommandSenderType2::Player; }
  
  bool isServer() const { return mType == CommandSenderType2::Server; }
  
  bool isNotPlayer() const { return mType != CommandSenderType2::Player; }
  
  bool isNotServer() const { return mType != CommandSenderType2::Server; }
  // umm... why not ?
  
  std::string getName() const;
  
  void sendMessage(const std::string& message);

private:
  void* mPtr;
  CommandSenderType2 mType;
  
  template <typename T, CommandSenderType2 p>
  T& get() {
    if (mType == p) {
      return *reinterpret_cast<T*>(mPtr);
    }
    throw std::runtime_error("Failed cast in CommandSender");
  }
  
  template <typename T, CommandSenderType2 p>
  const T& get() const {
    if (mType == p) {
      return *reinterpret_cast<const T*>(mPtr);
    }
    throw std::runtime_error("Failed cast in CommandSender");
  }
};

} // namespace redi

#endif // REDI_CHAT_COMMANDSENDER2_HPP