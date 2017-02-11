#ifndef REDI_SESSION
#define REDI_SESSION

#include <atomic>
#include <memory>
#include <boost/asio.hpp>
#include "threadsafequeue.hpp"
#include "sizeliteraloperators.hpp"
#include "bytebuffer.hpp"
#include "enums.hpp"
#include "protocol/packets/packethandler.hpp"

namespace redi
{

class Player;
class Server;
class Session;

using SessionUniquePtr = std::unique_ptr<Session>;
using SessionSharedPtr = std::shared_ptr<Session>;

class Session : public std::enable_shared_from_this<Session>
{
public:

  friend class Server;
  friend class Player;
  friend class PacketHandler;

  Session(boost::asio::ip::tcp::socket&& socket, Server& server);
  Session(const Session&) = delete;
  Session(Session&& s) = delete;
  ~Session();

  Session& operator=(const Session&) = delete;
  Session& operator=(Session&&) = delete;

  void sendPacket(ByteBuffer&& pkt, const std::string& message);
  void sendPacket(ByteBufferSharedPtr ptr);

  boost::asio::ip::tcp::socket& getSocket() { return mSocket; }
  const boost::asio::ip::tcp::socket& getSocket() const { return mSocket; }
  // And no, I don't have any idea why would I need a socket constant
  // Remember to add a reason here if you ever do this

  boost::asio::io_service& getIoService() { return mSocket.get_io_service(); }

  Server& getServer() { return mServer; }
  const Server& getServer() const { return mServer; }

  Player& getPlayer() { return *mPlayer; }
  void setPlayer(Player& player);

  ConnectionState getConnectionState() const { return mConnectionState; }
  void setConnectionState(ConnectionState s) { mConnectionState = s; }

  bool getCompressionIsSent() const { return mSetCompressionIsSent; }
  void setCompressionIsSent(bool b) { mSetCompressionIsSent = b; }

  void disconnect();
  
  bool isDisconnecting() const { return mIsDisconnecting; }

private:

  using PacketPtr = std::shared_ptr<ByteBuffer>;
  using PacketQueue = ThreadSafeQueue<PacketPtr>;

  boost::asio::ip::tcp::socket mSocket;
  PacketQueue mSendingQueue;
  PacketPtr mSendingPacket;
  ByteBuffer mReceivingPacket;
  Server& mServer;
  Player* mPlayer;
  ConnectionState mConnectionState;
  std::atomic_bool mSetCompressionIsSent;
  std::uint8_t mReceivingPacketSize[5];
  std::uint8_t mReceivingPacketCountSize;
  PacketHandlerSharedPtr mPacketHandler;
  std::atomic_bool mIsDisconnecting;

public:
  
  friend void sessionHandleRead(SessionSharedPtr ptr, const boost::system::error_code& error, bool header);
  void readNext();
  friend void sessionHandleWrite(SessionSharedPtr ptr, const boost::system::error_code& error);
  void writeNext();
};

inline bool operator==(const Session& l, const Session& r)
{
  return &l == &r;
}

inline bool operator!=(const Session& l, const Session& r)
{
  return !(l == r);
}

} // namespace redi

#endif // REDI_SESSION