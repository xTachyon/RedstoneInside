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

Session::Session(boost::asio::ip::tcp::socket&& socket, Server& server)
      : mSocket(std::move(socket)), mProtocol(std::make_unique<Protocol1_11>(*this)), mServer(server), mPlayer(nullptr), mConnectionState(ConnectionState::Handshake), mSetCompressionIsSent(false)
{
  readNext();
}

Session::~Session()
{
  Logger::info((boost::format("Session %1% destroyed") % this).str());
}

void Session::writeNext()
{
  if (mSendingPacket || mSendingQueue.empty()) return;
  
  mSendingPacket = mSendingQueue.pop();
  if (!mSendingPacket) return;
  
  asio::async_write(mSocket, asio::buffer(mSendingPacket->data(), mSendingPacket->size()),
                    boost::bind(&Session::handleWrite, this, asio::placeholders::error));
  // Crash sometimes here when trying to access a dead object
  // TODO: solve this
}

void Session::handleWrite(const boost::system::error_code& error)
{
  if (error)
  {
    disconnect();
    Logger::error("Client dc'ed");
  }
  else
  {
    mSendingPacket = {};
    writeNext();
  }
}

void Session::readNext()
{
  asio::async_read(mSocket, asio::buffer(mReceivingPacketSize, 1),
                   asio::transfer_exactly(1), boost::bind(&Session::handleRead, this, asio::placeholders::error, true));
  mReceivingPacketCountSize = 0;
}

void Session::handleRead(const boost::system::error_code& error, bool header)
{
  if (error)
  {
    disconnect();
    Logger::error("Client dc'ed");
  }
  else if (header)
  {
    ++mReceivingPacketCountSize;
    
    if ((mReceivingPacketSize[mReceivingPacketCountSize - 1] & 0b10000000) != 0)
    {
      if (mReceivingPacketCountSize > 5) static_cast<void>(header); // TODO: disconnect connection
      else asio::async_read(mSocket, asio::buffer(mReceivingPacketSize + mReceivingPacketCountSize, 1),
                            asio::transfer_exactly(1), boost::bind(&Session::handleRead, this, asio::placeholders::error, true));
    }
    else
    {
      PacketReader reader(ByteBuffer(mReceivingPacketSize, mReceivingPacketCountSize));
      std::size_t len = static_cast<std::size_t>(reader.readVarInt());
      mReceivingPacket.resize(len);
      asio::async_read(mSocket, asio::buffer(mReceivingPacket.data(), len),
                       asio::transfer_exactly(len), boost::bind(&Session::handleRead, this, asio::placeholders::error, false));
    }
  }
  else
  {
    mServer.addPacket(mProtocol.get(), std::move(mReceivingPacket));
    readNext();
  }
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