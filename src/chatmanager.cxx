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
  
  nlohmann::json j;
  
  j["text"] = "";
  j["extra"] = {};
  
  j["extra"].push_back(nlohmann::json(
        {
              {"text", (boost::format("[%1%]") % util::toString(&event.player)).str()},
              {"color", "light_purple"}
        }));
  j["extra"].push_back(nlohmann::json(
        {
              {"text", event.player.getPlayerName()},
              {"color", "aqua"}
        }));
  j["extra"].push_back(nlohmann::json(
        {
              {"text", ": "},
              {"color", "yellow"}
        }));
  j["extra"].push_back(nlohmann::json(
        {
              {"text", event.message},
              {"color", "red"}
        }));
  
  broadcastMessage(j, Server::toAllPlayers);
}

void ChatManager::broadcastMessage(nlohmann::json& j, std::function<bool(const Player&)> comp, ChatPosition position)
{
  for (Player& player : mServer.getOnlinePlayers())
  {
    if (comp(player))
    {
      ByteBuffer buf(player.getSession().getProtocol().createChatPacket(j.dump(), position));
      ByteBufferSharedPtr ptr(std::make_shared<ByteBuffer>(std::move(buf)));
      
      player.sendPacket(ptr);
    }
  }
}

void ChatManager::operator()(const Event&)
{
  
}
  
} // namespace redi