#ifndef REDI_CHATMANAGER_HPP
#define REDI_CHATMANAGER_HPP

#include <json.hpp>
#include "../enums.hpp"
#include "commandmanager.hpp"

namespace redi {

struct EventChatMessage;
struct EventPlayerJoin;
struct EventPlayerDisconnect;
class Player;
class Server;

struct ChatMessagePart {
  std::string message;
  std::string color;
  bool bold;
  bool italic;
  bool underlined;
  bool strikethrough;
  bool obfuscated;

  ChatMessagePart(std::string message = "", std::string color = "white",
                  bool bold = false, bool italic = false,
                  bool underlined = false, bool strikethrough = false,
                  bool obfuscated = false)
      : message(std::move(message)), color(std::move(color)), bold(bold),
        italic(italic), underlined(underlined), strikethrough(strikethrough),
        obfuscated(obfuscated) {}
};

using ChatComponent = std::vector<ChatMessagePart>;

class ChatManager {
public:
  ChatManager(Server& server, CommandManager& cmdmanager);

  void operator()(const std::string& message);
  void operator()(EventChatMessage&);
  void operator()(const EventPlayerJoin&);
  void operator()(const EventPlayerDisconnect&);

  void broadcastMessage(const std::string& message,
                        std::function<bool(const Player&)> comp,
                        ChatPosition position = ChatPosition::ChatBox);
  void broadcastJSONMessage(const std::string& json,
                            std::function<bool(const Player&)> comp,
                            ChatPosition position = ChatPosition::ChatBox);

  static std::string componentToJson(const ChatComponent& comp);

private:
  Server& mServer;
  CommandManager& mCmdManager;
};

} // namespace redi

#endif // REDI_CHATMANAGER_HPP