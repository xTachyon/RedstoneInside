#include <json.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/format.hpp>
#include "chatmanager.hpp"
#include "../server.hpp"

namespace redi {

ChatManager::ChatManager(Server& server)
    : HasServer(server), cmdmanager(server.getCommandManager()) {}

void ChatManager::operator()(const std::string& message) {
  broadcastMessage(message, Server::toAllPlayers);
  Logger::info(message);
}

void ChatManager::operator()(EventChatMessage& event) {
  string_view message = util::trim(string_view(event.message));
  
  if (message.size() == 0) {
    return;
  }
  if (message[0] == '/') {
    cmdmanager(event.player, message);
    return;
  }
  
  std::string json = event.player.getUsername() + ": " + event.message;

  broadcastJSONMessage(json, Server::toAllPlayers);
  Logger::info(
      (boost::format("%1%: %2%") % event.player.getUsername() % event.message)
          .str());
}

void ChatManager::operator()(const EventPlayerJoin& event) {
  auto& player = event.session->getPlayer();
  std::string message(
      (boost::format("%1% has joined the game") % player.getUsername()).str());
  Logger::info(message);
  broadcastJSONMessage(message,
                       Server::toAllPlayers, ChatPosition::SystemMessage);
}

void ChatManager::operator()(const EventPlayerDisconnect& event) {
  std::string message(
      (boost::format("%1% has left the game") % event.player.getUsername())
          .str());
  Logger::info(message);
  broadcastJSONMessage(message,
                       std::bind(Server::toAllPlayersExcept, std::placeholders::_1,
                std::ref(event.player)),
                       ChatPosition::SystemMessage);
}

void ChatManager::broadcastMessage(const std::string& message,
                                   std::function<bool(const Player&)> comp,
                                   ChatPosition position) {
  broadcastJSONMessage(message, comp,
                       position);
}

void ChatManager::broadcastJSONMessage(const std::string& json,
                                       std::function<bool(const Player&)> comp,
                                       ChatPosition position) {
for (
PlayerSharedPtr& player :
server.

getOnlinePlayers()

) {
    if (comp(*player)) {
      player->sendJSONMessage(json, position);
    }
  }
}

} // namespace redi