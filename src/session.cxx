#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include "session.hpp"
#include "logger.hpp"
#include "protocol/packetreader.hpp"
#include "server.hpp"
#include "protocol/packets/server/play/disconnect.hpp"

namespace asio = boost::asio;

namespace redi {

Session::Session(asio::ip::tcp::socket&& socket, Server& server)
    : HasServer(server), mSocket(std::move(socket)), mPlayer(nullptr),
      mConnectionState(ConnectionState::Handshake),
      mSetCompressionIsSent(false),
      mPacketHandler(std::make_shared<PacketHandler>(
          server, *this, server.getEventManager())),
      isDisconnected(false), mIsWritting(false),
      mStrand(mSocket.get_io_service()) {
  Logger::debug((boost::format("Session %1% created") % this).str());
}

Session::~Session() {
  Logger::debug((boost::format("Session %1% destroyed") % this).str());
}

void Session::handleWrite(const boost::system::error_code& error) {
  if (error) {
    disconnect();
  } else {
    mIsWritting = false;
  
    writeNext();
  }
}

void Session::postWrite() {
  if (mIsWritting || !mPacketsToBeSend.pop(mSendingPacket)) {
    return;
  }

  mIsWritting = true;

  asio::async_write(mSocket,
                    asio::buffer(mSendingPacket.data(), mSendingPacket.size()),
                    boost::bind(&Session::handleWrite, shared_from_this(),
                                asio::placeholders::error));
}

void Session::writeNext() {
  mSocket.get_io_service().post(
      mStrand.wrap([me = shared_from_this()] { me->postWrite(); }));
}

void Session::handleRead(const boost::system::error_code& error, bool header) {
  if (error) {
    disconnect();
  } else if (header) {
    ++mReceivingPacketCountSize;
  
    if ((mReceivingPacketSize[mReceivingPacketCountSize - 1] &
         0b10000000) != 0) {
      if (mReceivingPacketCountSize > 5)
        static_cast<void>(header); // TODO: disconnect connection
      else
        asio::async_read(mSocket,
                         asio::buffer(mReceivingPacketSize +
                                      mReceivingPacketCountSize,
                                      1),
                         asio::transfer_exactly(1),
                         boost::bind(&Session::handleRead, shared_from_this(),
                                     asio::placeholders::error, true));
    } else {
      PacketReader reader(ByteBuffer(mReceivingPacketSize,
                                     mReceivingPacketCountSize));
      std::size_t len = static_cast<std::size_t>(reader.readVarInt());
      mReceivingPacket.resize(len);
      asio::async_read(mSocket,
                       asio::buffer(mReceivingPacket.data(), len),
                       asio::transfer_exactly(len),
                       boost::bind(&Session::handleRead, shared_from_this(),
                                   asio::placeholders::error, false));
    }
  } else {
    try {
      mPacketHandler->readRaw(mReceivingPacket);
      server.addPacket(mPacketHandler);
    } catch (std::exception& e) {
      if (mPlayer) {
        mPlayer->kick(e.what());
      } else {
        disconnect();
      }
    }
    readNext();
  }
}

void Session::readNext() {
  asio::async_read(mSocket, asio::buffer(mReceivingPacketSize, 1),
                   asio::transfer_exactly(1),
                   boost::bind(&Session::handleRead, shared_from_this(),
                               asio::placeholders::error, true));
  mReceivingPacketCountSize = 0;
}

void Session::disconnect() {
  if (!isDisconnected) {
    isDisconnected = true;
    if (mPlayer)
      mPlayer->disconnect();

    EventUniquePtr ptr;
    if (mPlayer == nullptr)
      ptr = std::make_unique<EventSessionDisconnect>(*this);
    else
      ptr = std::make_unique<EventPlayerDisconnect>(*mPlayer);
    server.addEvent(std::move(ptr));
  }
}

void Session::kick(const std::string& message) {
  packets::Disconnect(ChatManager::componentToJson(
                          ChatComponent({ChatMessagePart(std::move(message))})),
                      mConnectionState == ConnectionState::Play)
      .send(*this);
  disconnect();
}

void Session::setPlayer(Player& player) { mPlayer = std::addressof(player); }

void Session::sendPacket(ByteBuffer&& packet, const std::string& message) {
  //  Logger::debug(message);
  //  static_cast<void>(message);
  //
  //  std::ostringstream ss;
  //  ss << '\n' << message << '\n';
  //  for (auto& c : pkt) ss << (int)c << ' ';
  //  ss << '\n';
  //  Logger::debug(ss.str());
  static_cast<void>(message);

  mPacketsToBeSend.push(std::move(packet));
  writeNext();
}

void Session::sendPacket(const ByteBuffer& packet, const std::string& message) {
  static_cast<void>(message);

  mPacketsToBeSend.push(std::move(packet));
  writeNext();
}

} // namespace redi