#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "session.hpp"
#include "logger.hpp"

namespace asio = boost::asio;

namespace redi
{

Session::Session(boost::asio::ip::tcp::socket&& socket)
      : mSocket(std::move(socket)) {}

Session::Session(Session&& s)
      : mSocket(std::move(s.mSocket)) {}

void Session::handleRead(const boost::system::error_code& error, std::size_t bytes)
{
  if (error) Logger::warn(error.message());
  
  mReceivingPacket.resize(bytes);
  readNext();
}

void Session::readNext()
{
  mReceivingPacket.resize(MaximumTcpPackeSize);
  asio::async_read(mSocket,
                   asio::buffer(mReceivingPacket.data(), mReceivingPacket.size()),
                                boost::bind(&Session::handleRead, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

void Session::writeNext()
{
  mSendingPacket = mPacketsToBeSend.pop();
  protocol::Packet& pkt = *mSendingPacket;
  
  asio::async_write(mSocket,
                    asio::buffer(pkt.data.data() + pkt.startsAt,
                                 pkt.data.size() - pkt.startsAt),
                    boost::bind(&Session::handleWrite, this, asio::placeholders::error));
}

void Session::handleWrite(const boost::system::error_code& error)
{
  if (error) Logger::warn(error.message());
  writeNext();
}
  
} // namespace redi