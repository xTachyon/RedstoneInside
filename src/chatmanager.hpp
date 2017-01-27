#ifndef REDI_CHATMANAGER_HPP
#define REDI_CHATMANAGER_HPP

#include <json.hpp>
#include "enums.hpp"

namespace redi
{

class Event;
class EventChatMessage;
class Player;
class Server;

class ChatManager
{
public:
  
  ChatManager(Server&);
  
  void operator()(const EventChatMessage&);
  void operator()(const Event&);
  
  void broadcastMessage(nlohmann::json& j, std::function<bool(const Player&)> comp, ChatPosition position = ChatPosition::ChatBox);
  
private:
  
  Server& mServer;
};
  
} // namespace redi

#endif // REDI_CHATMANAGER_HPP