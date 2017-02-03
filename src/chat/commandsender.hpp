#ifndef REDI_CHAT_COMMANDSENDER_HPP
#define REDI_CHAT_COMMANDSENDER_HPP

#include "../player.hpp"

namespace redi
{

enum class CommandSenderType
{
  Player,
  Server
};

class CommandSender
{
public:
  
  CommandSender(Player& player) : mPtr(&player), mType(CommandSenderType::Player) {}
  CommandSender(Server& server) : mPtr(&server), mType(CommandSenderType::Server) {}
  
  Player& getPlayer() { return get<Player, CommandSenderType::Player>(); }
  const Player& getPlayer() const { return get<Player, CommandSenderType::Player>(); }
  
  Server& getServer() { return get<Server, CommandSenderType::Server>(); }
  const Server& getServer() const { return get<Server, CommandSenderType::Server>(); }
  
  CommandSenderType getType() const { return mType; }
  
  bool isPlayer() const { return mType == CommandSenderType::Player; }
  bool isServer() const { return mType == CommandSenderType::Server; }
  
  bool isNotPlayer() const { return mType != CommandSenderType::Player; }
  bool isNotServer() const { return mType != CommandSenderType::Server; }
  // umm... why not ?
  
  std::string getName() const;
  
  void sendMessage(const std::string& message);
  
private:
  
  void* mPtr;
  CommandSenderType mType;
  
  template <typename T, CommandSenderType p>
  T& get()
  {
    if (mType == p) return *reinterpret_cast<T*>(mPtr);
    throw std::runtime_error("Failed cast in CommandSender");
  }
  
  template <typename T, CommandSenderType p>
  const T& get() const
  {
    if (mType == p) return *reinterpret_cast<const T*>(mPtr);
    throw std::runtime_error("Failed cast in CommandSender");
  }
};
  
} // namespace redi

#endif // REDI_CHAT_COMMANDSENDER_HPP