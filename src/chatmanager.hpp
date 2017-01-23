#ifndef REDI_CHATMANAGER_HPP
#define REDI_CHATMANAGER_HPP

namespace redi
{

class Server;
class EventChatMessage;

class ChatManager
{
public:
  
  ChatManager(Server&);
  
  void operator()(const EventChatMessage&);
  
private:
  
  Server& mServer;
};
  
} // namespace redi

#endif // REDI_CHATMANAGER_HPP