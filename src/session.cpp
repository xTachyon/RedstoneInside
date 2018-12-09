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

Session::Session(SocketSharedPtr socket, Server& server)
  : HasServer(server), socket(socket),
    player(nullptr),
  connectionState(ConnectionState::Handshake),
  setCompressionIsSentVar(false),
  packetHandler(std::make_shared<PacketHandler>(
    server, *this, server.getEventManager())),
  isDisconnected(false), isWritting(false) {
  Logger::debug((boost::format("Session %1% created") % this).str());

  socket->set_read_handler([this] (auto bytes, auto error) { this->on_read(bytes, error); });
  socket->read(MutableBuffer(reading_buffer, sizeof(reading_buffer)));
}

Session::~Session() {
  Logger::debug((boost::format("Session %1% destroyed") % this).str());
}

void Session::on_read(size_t bytes, std::string error) {
  if (!error.empty()) {
    disconnect();
    return;
  }
  reading_buffer_vector.append(reading_buffer, bytes);
  deserialize_packets();
  socket->read(MutableBuffer(reading_buffer, sizeof(reading_buffer)));
}

void Session::disconnect() {
  bool disconnected = isDisconnected.exchange(true);
  if (disconnected) {
    return;
  }
  socket->close();

  if (player) {
    player->disconnect();
    server.addEvent(std::make_unique<EventPlayerDisconnect>(*player));
  }

  server.addEvent(std::make_unique<EventSessionDisconnect>(*this));
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

  std::array<ConstBuffer, 2> buffers;
  buffers[0] = { result.first.data(), result.second };
  buffers[1] = packet;
  socket->write(buffers.data(), buffers.size());
}

void Session::deserialize_packets() {
  while (protocol::varint::is_complete(ConstBuffer(reading_buffer_vector))) {
    PacketReader reader((ConstBuffer(reading_buffer_vector)));
    auto size = static_cast<size_t>(reader.readVarInt());
    auto varint_size = protocol::varint::varint_size(size);
    if (size + varint_size > reading_buffer_vector.size()) {
      return;
    }
    ConstBuffer buffer(reading_buffer_vector.data() + varint_size, size);
    packetHandler->readRaw(buffer);
    server.addTask([handler = this->packetHandler] {
      handler->handleOne();
    } );

    reading_buffer_vector.erase(reading_buffer_vector.begin(),
        reading_buffer_vector.begin() + varint_size + size);
  }
}

} // namespace redi