#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "../../session.hpp"
#include "packethandler.hpp"
#include "../../util/util.hpp"
#include "../../server.hpp"
#include "../../player.hpp"

namespace asio = boost::asio;

namespace redi {

PacketHandler::PacketHandler(Server& server, Session& session, EventManager&)
    : mServer(server), mSession(session) {}

void PacketHandler::addPacket(PacketUniquePtr&& packet) {
  std::lock_guard<std::mutex> l(mMutex);
  mPackets.emplace_back(std::move(packet));
}

void PacketHandler::readRaw(ConstBuffer buffer) {
  PacketReader packet(buffer);
  std::int32_t type = packet.readVarInt();
  PacketUniquePtr ptr;

  switch (mSession.getConnectionState()) {
  case ConnectionState::Handshake: {
    switch (type) {
    case 0x00: {
      Handshake handshake(packet);
      handleHandshake(handshake);
      return;
    } break;

    default:
      break;
    }
  }
    break;

  case ConnectionState::Login: {
    switch (type) {
    case 0x00: {
      packets::LoginStart ls(packet);
      handleLoginStart(ls);
      return;
    }

    default:
      break;
    }
  } break;

  case ConnectionState::Status: {
    switch (type) {
    case 0x00:
      ptr = std::make_unique<packets::Request>(packet);
      break;

    case 0x01:
      ptr = std::make_unique<packets::Ping>(packet);
      break;

    default:
      break;
    }
  } break;

  case ConnectionState::Play: {
    switch (type) {
    case 0x02:
      ptr = std::make_unique<packets::ChatMessage>(packet);
      break;

    case 0x0D:
      ptr = std::make_unique<packets::PlayerPositionAndLook>(packet);
      break;

    case 0x0C:
      ptr = std::make_unique<packets::PlayerPosition>(packet);
      break;

    case 0x0E:
      ptr = std::make_unique<packets::PlayerLook>(packet);
      break;

    default:
      break;
    }
  } break;
  }

  addPacket(std::move(ptr));
}

void PacketHandler::handleOne() {
  PacketUniquePtr ptr;

  {
    std::lock_guard<std::mutex> l(mMutex);
    if (mPackets.empty())
      return;

    ptr = std::move(mPackets.front());
    mPackets.pop_front();
  }

  try {
    if (ptr) {
      ptr->process(*this);
    }
  } catch (std::exception& e) {
    mSession.kick(e.what());
  }
}

void PacketHandler::handleHandshake(Handshake& p) {
  mSession.connectionState = p.state;
}

void PacketHandler::handleStatusRequest(packets::Request&) {
  packets::Response(mServer).send(mSession);
}

void PacketHandler::handleStatusPing(packets::Ping& packet) {
  packets::Pong(packet.payload).send(mSession);
  Server& server = mSession.getServer();
  auto timer = std::make_shared<boost::asio::steady_timer>(server.getWorkIO(), asio::chrono::seconds(10));
  timer->async_wait([timer, session = mSession.shared_from_this()] (const boost::system::error_code&) {
    session->disconnect();
  });
}

void PacketHandler::handleLoginStart(packets::LoginStart& packet) {
  mServer.addEvent(std::make_unique<EventPlayerJoin>(
      mSession.shared_from_this(), std::move(packet.username)));
}

void PacketHandler::handleChatMessage(packets::ChatMessage& packet) {
  mServer.addEvent(std::make_unique<EventChatMessage>(
      mSession.getPlayer(), std::move(packet.message)));
}

void PacketHandler::handlePlayerPositionAndLook(
    packets::PlayerPositionAndLook& packet) {
  PlayerPosition position = mSession.getPlayer().mPosition;

  position.x = packet.x;
  position.y = packet.y;
  position.z = packet.z;
  position.yaw = packet.yaw;
  position.pitch = packet.pitch;
  position.onGround = packet.onGround;
  mSession.getPlayer().onEntityMovedWithLook(position);
  mSession.getPlayer().mPosition = position;
  mSession.getPlayer().normalizeRotation();
  mSession.getPlayer().onPositionChanged();
}

void PacketHandler::handlePlayerPosition(packets::PlayerPosition& packet) {
  PlayerPosition& position = mSession.getPlayer().mPosition;

  position.x = packet.x;
  position.y = packet.y;
  position.z = packet.z;
  position.onGround = packet.onGround;
  mSession.getPlayer().onPositionChanged();
}

void PacketHandler::handlePlayerLook(packets::PlayerLook& packet) {
  PlayerPosition& position = mSession.getPlayer().mPosition;

  position.yaw = packet.yaw;
  position.pitch = packet.pitch;
  position.onGround = packet.onGround;
  mSession.getPlayer().normalizeRotation();
}

} // namespace redi