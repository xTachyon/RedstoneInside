#pragma once

#include "../../threadsafequeue.hpp"
#include "server/handshakepackets.hpp"
#include "server/statuspackets.hpp"
#include "server/loginpackets.hpp"
#include "server/playpackets.hpp"

namespace redi {

class Server;
class EventManager;

class PacketHandler {
public:
  PacketHandler(Server& server, Session& session, EventManager& eventh);

  void addPacket(PacketUniquePtr&& packet);
  void handleOne();
  void readRaw(ConstBuffer buffer);

  void handleHandshake(packets::Handshake& packet);

  void handleStatusRequest(packets::Request& packet);
  void handleStatusPing(packets::Ping& packet);

  void handleLoginStart(packets::LoginStart& packet);
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