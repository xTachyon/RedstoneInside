#pragma once

#include "../../threadsafequeue.hpp"
#include "server/handshake/handshake.hpp"
#include "server/login/loginstart.hpp"
#include "server/status/request.hpp"
#include "server/status/ping.hpp"
#include "server/play/chatmessage.hpp"
#include "server/play/playerpositionandlook.hpp"
#include "server/play/playerposition.hpp"
#include "server/play/playerlook.hpp"

namespace redi {

class Server;
class EventManager;

class PacketHandler {
public:
  PacketHandler(Server& server, Session& session, EventManager& eventh);

  void addPacket(PacketUniquePtr&& packet);
  void handleOne();
  void readRaw(ConstBuffer buffer);

  void handleHandshake(Handshake& packet);

  void handleStatusRequest(Request& packet);
  void handleStatusPing(Ping& packet);

  void handleLoginStart(LoginStart& packet);
  void handleChatMessage(packets::ChatMessage& packet);
  void handlePlayerPositionAndLook(packets::PlayerPositionAndLook& packet);
  void handlePlayerPosition(packets::PlayerPosition& packet);
  void handlePlayerLook(packets::PlayerLook& packet);

  Session& getSession() { return mSession; }
private:
  Server& mServer;
  Session& mSession;
  std::deque<PacketUniquePtr> mPackets;
  std::mutex mMutex;
};

using PacketHandlerSharedPtr = std::shared_ptr<PacketHandler>;

} // namespace redi