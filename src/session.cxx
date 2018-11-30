#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include "session.hpp"
#include "logger.hpp"
#include "protocol/packetreader.hpp"
#include "server.hpp"
#include "protocol/packets/server/play/disconnect.hpp"
#include "protocol/packetwriter.hpp"
#include "protocol/varint.hpp"

namespace asio = boost::asio;

namespace redi {

Session::Session(asio::ip::tcp::socket&& socket, Server& server)
  : HasServer(server), socket(std::move(socket)),
    player(nullptr),
  connectionState(ConnectionState::Handshake),
  setCompressionIsSentVar(false),
  packetHandler(std::make_shared<PacketHandler>(
    server, *this, server.getEventManager())),
  isDisconnected(false), isWritting(false),
  strand(socket.get_io_service()) {
  Logger::debug((boost::format("Session %1% created") % this).str());
}

Session::~Session() {
  Logger::debug((boost::format("Session %1% destroyed") % this).str());
}

void Session::handleWrite(const boost::system::error_code& error) {
  if (error) {
    disconnect();
  } else {
    isWritting = false;

    writeNext();
  }
}

void Session::postWrite() {
  if (isWritting || !packetsToBeSend.pop(sendingPacket)) {
    return;
  }

  isWritting = true;

  asio::async_write(socket,
    asio::buffer(sendingPacket.data(), sendingPacket.size()),
    boost::bind(&Session::handleWrite, shared_from_this(),
      asio::placeholders::error));
}

void Session::writeNext() {
  socket.get_io_service().post(
    strand.wrap([me = shared_from_this()]{ me->postWrite(); }));
}

void Session::handleRead(const boost::system::error_code& error, bool header) {
  if (error) {
    disconnect();
  } else if (header) {
    ++receivingPacketCountSize;

    if ((receivingPacketSize[receivingPacketCountSize - 1] &
      0b10000000) != 0) {
      if (receivingPacketCountSize > 5) {
        disconnect();
      } else {
        asio::async_read(socket,
          asio::buffer(receivingPacketSize +
            receivingPacketCountSize,
            1),
          asio::transfer_exactly(1),
          boost::bind(&Session::handleRead, shared_from_this(),
            asio::placeholders::error, true));
      }
    } else {
      PacketReader reader(ByteBuffer(receivingPacketSize,
        receivingPacketCountSize));
      std::size_t len = static_cast<std::size_t>(reader.readVarInt());
      receivingPacket.resize(len);
      asio::async_read(socket,
        asio::buffer(receivingPacket.data(), len),
        asio::transfer_exactly(len),
        boost::bind(&Session::handleRead, shared_from_this(),
          asio::placeholders::error, false));
    }
  } else {
    try {
      packetHandler->readRaw(receivingPacket);
      server.addPacket(packetHandler);
    } catch (std::exception& e) {
      if (player) {
        player->kick(e.what());
      } else {
        disconnect();
      }
    }
    readNext();
  }
}

void Session::readNext() {
  asio::async_read(socket, asio::buffer(receivingPacketSize, 1),
    asio::transfer_exactly(1),
    boost::bind(&Session::handleRead, shared_from_this(),
      asio::placeholders::error, true));
  receivingPacketCountSize = 0;
}

void Session::disconnect() {
  bool disc = isDisconnected.exchange(true);
  if (!disc) {
    if (player)
      player->disconnect();

    EventUniquePtr ptr;
    if (player == nullptr)
      ptr = std::make_unique<EventSessionDisconnect>(*this);
    else
      ptr = std::make_unique<EventPlayerDisconnect>(*player);
    server.addEvent(std::move(ptr));
  }
}

void Session::kick(const std::string& message) {
  packets::Disconnect(message,
    connectionState == ConnectionState::Play)
    .send(*this);
  disconnect();
}

void Session::setPlayer(Player& player) { this->player = std::addressof(player); }

void Session::sendPacket(ByteBuffer packet, const std::string& message) {
  static_cast<void>(message);

  auto result = protocol::varint::encodeVarInt(packet.size());

  packetsToBeSend.push(ConstBuffer(result.first.data(), result.second).toByteBuffer(), std::move(packet));
  writeNext();
}

} // namespace redi