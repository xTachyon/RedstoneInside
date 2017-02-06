#ifndef REDI_PACKETHANDLER_HPP
#define REDI_PACKETHANDLER_HPP

#include "../../threadsafequeue.hpp"
#include "server/handshake/handshake.hpp"
#include "server/login/loginstart.hpp"
#include "server/status/request.hpp"
#include "server/status/ping.hpp"
#include "server/play/chatmessage.hpp"

namespace redi
{

class Server;
class EventManager;

class PacketHandler
{
public:
  
  PacketHandler(Server& server, Session& session, EventManager& eventh);
  
  void addPacket(PacketUniquePtr&& packet);
  void handleOne();
  void readRaw(ByteBuffer buffer);
  
  void handleHandshake(Handshake& packet);
  
  void handleStatusRequest(Request& packet);
  void handleStatusPing(Ping& packet);
  
  void handleLoginStart(LoginStart& packet);
  void handleChatMessage(packets::ChatMessage& packet);
  
private:
  
  Server& mServer;
  Session& mSession;
  EventManager& mEventManager;
  std::deque<PacketUniquePtr> mPackets;
  std::mutex mMutex;
};

using PacketHandlerSharedPtr = std::shared_ptr<PacketHandler>;
  
} // namespace redi

#endif // REDI_PACKETHANDLER_HPP