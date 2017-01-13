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

Session::Session(boost::asio::ip::tcp::socket&& socket, Server* server)
      : state(State::Handshake), setCompressionSent(false), mSocket(std::move(socket)), mProtocol(new Protocol1_11(this)), mServer(server), mErrors(0)
{
  readNext();
}

Session::Session(Session&& s)
      : state(State::Login), mSocket(std::move(s.mSocket))
{
  readNext();
}

Session::~Session()
{
  while (mErrors < 1);
  Logger::info((boost::format("Session %1% destroyed") % this).str());
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
    ++mErrors;
    kill();
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
    ++mErrors;
    kill();
    Logger::error("Client dc'ed");
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
  EventPtr ptr(new EventSessionDC(this));
  mServer->addEvent(ptr);
}

void Session::setPlayer(Player& player)
{
  mPlayer = std::addressof(player);
}
  
} // namespace redi