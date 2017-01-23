#include <json.hpp>
#include <boost/format.hpp>
#include "chatmanager.hpp"
#include "server.hpp"
#include "events/events.hpp"
#include "protocol/packetwriter.hpp"
#include "logger.hpp"

namespace redi
{

ChatManager::ChatManager(Server& server) : mServer(server) {}

void ChatManager::operator()(const EventChatMessage& event)
{
  nlohmann::json j;
  
  std::string finalmessage = (boost::format("[%3%]%1%: %2%") % event.player.getPlayerName() % event.message % &event.player).str();
  j["text"] = finalmessage;
  Logger::info(finalmessage);
  
  PacketWriter pkt(0x0F);
  pkt.writeString(j.dump());
  pkt.writeByte(0);
  pkt.commit();
  
  ByteBufferSharedPtr ptr = std::make_shared<ByteBuffer>(std::move(pkt.data));
  mServer.broadcastPacketToPlayers(ptr, Server::toAllPlayers);
}
  
} // namespace redi