#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include "session.hpp"
#include "logger.hpp"
#include "protocol/packetreader.hpp"
#include "server.hpp"

namespace asio = boost::asio;

enum class Items
{
  IronShovel = 256,
  IronPickaxe = 257
};

namespace redi
{

Session::Session(asio::ip::tcp::socket&& socket, Server& server)
      : mSocket(std::move(socket)), mServer(server), mPlayer(nullptr), mConnectionState(ConnectionState::Handshake),
        mSetCompressionIsSent(false), mPacketHandler(std::make_shared<PacketHandler>(mServer, *this, mServer.getEventManager())),
        mIsDisconnecting(false), mIsWritting(false), mStrand(mSocket.get_io_service())
{
  Logger::debug((boost::format("Session %1% created") % this).str());
}

Session::~Session()
{
  Logger::debug((boost::format("Session %1% destroyed") % this).str());
}

void sessionHandleWrite(SessionSharedPtr ptr, const boost::system::error_code& error)
{
  if (error)
  {
    ptr->disconnect();
  }
  else
  {
//    ptr->mSendingPacket = {};
    ptr->mIsWritting = false;
    //assert(!ptr->mIsWritting);
    
    ptr->writeNext();
  }
}

void Session::writeNext()
{
  if (mIsWritting || !mPacketsToBeSend.pop(mSendingPacket))
  {
    return;
  }
  
  //assert(!mIsWritting);
  mIsWritting = true;
  
  //assert(mIsWritting);
  asio::async_write(mSocket, asio::buffer(mSendingPacket.data(), mSendingPacket.size()),
                    boost::bind(sessionHandleWrite, shared_from_this(), asio::placeholders::error));
}

void sessionHandleRead(SessionSharedPtr ptr, const boost::system::error_code& error, bool header)
{
  if (error)
  {
    ptr->disconnect();
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
    ptr->mPacketHandler->readRaw(ptr->mReceivingPacket);
    ptr->mServer.addPacket(ptr->mPacketHandler);
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
  if (!mIsDisconnecting)
  {
    mIsDisconnecting = true;
    if (mPlayer) mPlayer->disconnect();
  
    EventSharedPtr ptr;
    if (mPlayer == nullptr) ptr = std::make_shared<EventSessionDisconnect>(*this);
    else ptr = std::make_shared<EventPlayerDisconnect>(*mPlayer);
    mServer.addEvent(ptr);
  }
}

void Session::setPlayer(Player& player)
{
  mPlayer = std::addressof(player);
}

void Session::sendPacket(ByteBuffer&& packet, const std::string& message)
{
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

void Session::sendPacket(const ByteBuffer& packet, const std::string& message)
{
  static_cast<void>(message);
  
  mPacketsToBeSend.push(std::move(packet));
  writeNext();
}

} // namespace redi