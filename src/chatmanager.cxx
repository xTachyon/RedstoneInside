#include <json.hpp>
#include <boost/format.hpp>
#include "chatmanager.hpp"
#include "server.hpp"
#include "events/events.hpp"
#include "protocol/packetwriter.hpp"
#include "logger.hpp"
#include "exceptions.hpp"
#include "util/util.hpp"

namespace redi
{

ChatManager::ChatManager(Server& server) : mServer(server) {}

void ChatManager::operator()(const EventChatMessage& event)
{
  if (event.message == "/stop") throw StopServer();
  else if (event.message == "/uuid")
  {
    event.player.sendMessage(event.player.getUUIDasString());
    return;
  }
  
  std::string json = componentToJson(ChatComponent
                                           {
                                                 ChatMessagePart((boost::format("[%1%]") % util::toString(&event.player)).str(), "light_purple"),
                                                 ChatMessagePart(event.player.getUsername(), "aqua"),
                                                 ChatMessagePart(": ", "yellow"),
                                                 ChatMessagePart(event.message, "red")
                                           });
  
  broadcastJSONMessage(json, Server::toAllPlayers);
  Logger::info((boost::format("[%3%]%1%: %2%") % event.player.getUsername() % event.message % &event.player).str());
}

void ChatManager::operator()(const EventPlayerJoin& event)
{
  Player& player = event.session.getPlayer();
  std::string message((boost::format("[%2%]%1% has joined the game") % player.getUsername() % &player).str());
  Logger::info(message);
  broadcastJSONMessage(componentToJson(ChatComponent
                                             {
                                                   ChatMessagePart(message, "green")
                                             }), Server::toAllPlayers, ChatPosition::SystemMessage);
}

void ChatManager::operator()(const EventPlayerDisconnect& event)
{
  std::string message((boost::format("[%2%]%1% has left the game") % event.player.getUsername() % &event.player).str());
  Logger::info(message);
  broadcastJSONMessage(componentToJson(ChatComponent
                                             {
                                                   ChatMessagePart(message, "green")
                                             }), std::bind(Server::toAllPlayersExcept, std::placeholders::_1, std::ref(event.player)), ChatPosition::SystemMessage);
}

void ChatManager::broadcastMessage(const std::string& message, std::function<bool(const Player&)> comp, ChatPosition position)
{
  broadcastJSONMessage(componentToJson(ChatComponent(
        {
              ChatMessagePart(message)
        })), comp, position);
}

void ChatManager::broadcastJSONMessage(const std::string& json, std::function<bool(const Player&)> comp, ChatPosition position)
{
  for (Player& player : mServer.getOnlinePlayers())
  {
    if (comp(player))
    {
//      ByteBuffer buf(player.getSession().getProtocol().createChatPacket(json, position));
//      ByteBufferSharedPtr ptr(std::make_shared<ByteBuffer>(std::move(buf)));
      
      player.sendJSONMessage(json, position);
    }
  }
}

std::string ChatManager::componentToJson(const ChatComponent& comp)
{
  nlohmann::json j;
  
  j["text"] = "";
  j["extra"] = {};
  
  for (const auto& i : comp)
  {
    nlohmann::json c;
    
    c["text"] = i.message;
    if (i.color != "white") c["color"] = i.color;
    if (i.bold) c["bold"] = i.bold;
    if (i.italic) c["italic"] = i.italic;
    if (i.underlined) c["underlined"] = i.underlined;
    if (i.strikethrough) c["strikethrough"] = i.strikethrough;
    if (i.obfuscated) c["obfuscated"] = i.obfuscated;
    
    j["extra"].push_back(c);
  }
  
  return j.dump();
}
  
} // namespace redi