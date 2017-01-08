#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "session.hpp"
#include "logger.hpp"
#include "protocol/protocol.hpp"
#include "protocol/protocol1_11.hpp"
#include "protocol/packetreader.hpp"
#include "server.hpp"

namespace asio = boost::asio;

namespace redi
{

Session::Session(boost::asio::ip::tcp::socket&& socket, Server* server)
      : stage(State::Handshake), mSocket(std::move(socket)), mServer(server)
{
  readNext();
}

Session::Session(Session&& s)
      : stage(State::Login), mSocket(std::move(s.mSocket))
{
  readNext();
}

void Session::writeNext()
{
  if (mSendingPacket || mSendingQueue.empty()) return;
  
  mSendingPacket = mSendingQueue.pop();
  asio::async_write(mSocket, asio::buffer(mSendingPacket->data(), mSendingPacket->size()),
                    boost::bind(&Session::handleWrite, this, asio::placeholders::error));
}

void Session::handleWrite(const boost::system::error_code& error)
{
  if (error)
  {
    Logger::error(error.message());
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
    Logger::error(error.message());
  }
  else if (header)
  {
    ++mReceivingPacketCountSize;
    
    if ((mReceivingPacketSize[mReceivingPacketCountSize - 1] & 0b10000000) != 0)
    {
      if (mReceivingPacketCountSize > 5) static_cast<void>(header); // TODO: kill connection
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
    mServer->addPacket(mProtocol.get(), std::move(mReceivingPacket));
    readNext();
  }
}

void Session::kill()
{
  mServer->killConnectedSession(this);
}

void Session::setProtocol(SessionPtr ptr)
{
  mProtocol.reset(new Protocol1_11(ptr));
}
  
} // namespace redi