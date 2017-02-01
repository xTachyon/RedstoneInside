#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include "session.hpp"
#include "logger.hpp"
#include "protocol/protocol.hpp"
#include "protocol/protocol1_11.hpp"
#include "protocol/packetreader.hpp"
#include "server.hpp"

namespace asio = boost::asio;

namespace redi
{

Session::Session(asio::ip::tcp::socket&& socket, Server& server)
      : mSocket(std::move(socket)), mProtocol(std::make_unique<Protocol1_11>(*this)), mServer(server), mPlayer(nullptr), mConnectionState(ConnectionState::Handshake), mSetCompressionIsSent(false) {}

Session::~Session()
{
  Logger::info((boost::format("Session %1% destroyed") % this).str());
}

void sessionHandleWrite(SessionSharedPtr ptr, const boost::system::error_code& error)
{
  if (error)
  {
    ptr->disconnect();
    Logger::error("Client dc'ed");
  }
  else
  {
    ptr->mSendingPacket = {};
    ptr->writeNext();
  }
}

void Session::writeNext()
{
  if (mSendingPacket || mSendingQueue.empty()) return;
  
  mSendingPacket = mSendingQueue.pop();
  if (!mSendingPacket) return;
  
  asio::async_write(mSocket, asio::buffer(mSendingPacket->data(), mSendingPacket->size()),
                    boost::bind(sessionHandleWrite, shared_from_this(), asio::placeholders::error));
}

void sessionHandleRead(SessionSharedPtr ptr, const boost::system::error_code& error, bool header)
{
  if (error)
  {
    ptr->disconnect();
    Logger::error("Client dc'ed");
  }
  else if (header)
  {
    ++ptr->mReceivingPacketCountSize;
    
    if ((ptr->mReceivingPacketSize[ptr->mReceivingPacketCountSize - 1] & 0b10000000) != 0)
    {
      if (ptr->mReceivingPacketCountSize > 5) static_cast<void>(header); // TODO: disconnect connection
      else asio::async_read(ptr->mSocket, asio::buffer(ptr->mReceivingPacketSize + ptr->mReceivingPacketCountSize, 1),
                            asio::transfer_exactly(1), boost::bind(sessionHandleRead, ptr, asio::placeholders::error, true));
    }
    else
    {
      PacketReader reader(ByteBuffer(ptr->mReceivingPacketSize, ptr->mReceivingPacketCountSize));
      std::size_t len = static_cast<std::size_t>(reader.readVarInt());
      ptr->mReceivingPacket.resize(len);
      asio::async_read(ptr->mSocket, asio::buffer(ptr->mReceivingPacket.data(), len),
                       asio::transfer_exactly(len), boost::bind(sessionHandleRead, ptr, asio::placeholders::error, false));
    }
  }
  else
  {
    ptr->mServer.addPacket(ptr->mProtocol.get(), std::move(ptr->mReceivingPacket));
    ptr->readNext();
  }
}

void Session::readNext()
{
  asio::async_read(mSocket, asio::buffer(mReceivingPacketSize, 1),
                   asio::transfer_exactly(1), boost::bind(sessionHandleRead, shared_from_this(), asio::placeholders::error, true));
  mReceivingPacketCountSize = 0;
}

void Session::disconnect()
{
  EventSharedPtr ptr;
  if (mPlayer == nullptr) ptr = std::make_shared<EventSessionDisconnect>(*this);
  else ptr = std::make_shared<EventPlayerDisconnect>(*mPlayer);
  mServer.addEvent(ptr);
}
void Session::setPlayer(Player& player)
{
  mPlayer = std::addressof(player);
}

void Session::sendPacket(ByteBuffer&& pkt, const char*)
{
  mSendingQueue.push(std::make_shared<ByteBuffer>(std::move(pkt)));
  writeNext();
}

void Session::sendPacket(ByteBufferSharedPtr ptr)
{
  mSendingQueue.push(ptr);
  writeNext();
}

} // namespace redi