#ifndef REDI_CHATMANAGER_HPP
#define REDI_CHATMANAGER_HPP

#include <json.hpp>
#include "../enums.hpp"
#include "../commands/commandmanager.hpp"

namespace redi {

struct EventChatMessage;
struct EventPlayerJoin;
struct EventPlayerDisconnect;
class Player;
class Server;

class ChatManager : public HasServer {
public:
  ChatManager(Server& server);

  void operator()(const std::string& message);
  void operator()(EventChatMessage&);
  void operator()(const EventPlayerJoin&);
  void operator()(const EventPlayerDisconnect&);
  
  void broadcast(const std::string& message, ChatPosition position = ChatPosition::ChatBox);
private:
  commands::CommandManager& cmdmanager;
};

} // namespace redi

#endif // REDI_CHATMANAGER_HPP