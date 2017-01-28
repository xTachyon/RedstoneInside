#ifndef REDI_SESSION
#define REDI_SESSION

#include <atomic>
#include <boost/asio.hpp>
#include "threadsafequeue.hpp"
#include "sizeliteraloperators.hpp"
#include "bytebuffer.hpp"
#include "protocol/protocol.hpp"
#include "enums.hpp"

namespace redi
{

class Server;

class Session
{
public:
  
  Session(boost::asio::ip::tcp::socket&& socket, Server& server);
  Session(const Session&) = delete;
  Session(Session&& s) = delete;
  ~Session();
  
  Session& operator=(const Session&) = delete;
  Session& operator=(Session&&) = delete;
  
  void sendPacket(ByteBuffer&& pkt, const char* message);
  void sendPacket(ByteBufferSharedPtr ptr);
  
  boost::asio::ip::tcp::socket& getSocket() { return mSocket; }
  const boost::asio::ip::tcp::socket& getSocket() const { return mSocket; }
  // And no, I don't have any idea why would I need a socket constant
  // Remember to add a reason here if you ever do this
  
  boost::asio::io_service& getIoService() { return mSocket.get_io_service(); }

  Server& getServer() { return mServer; }
  const Server& getServer() const { return mServer; }
  
  Protocol& getProtocol() { return *mProtocol; }
  const Protocol& getProtocol() const { return *mProtocol; }
  
  Player& getPlayer() { return *mPlayer; }
  void setPlayer(Player& player);
  
  ConnectionState getConnectionState() const { return mConnectionState; }
  void setConnectionState(ConnectionState s) { mConnectionState = s; }
  
  bool getCompressionIsSent() const { return mSetCompressionIsSent; }
  void setCompressionIsSent(bool b) { mSetCompressionIsSent = b; }
  
  void disconnect();
  
  private:
  
  using PacketPtr = std::shared_ptr<ByteBuffer>;
  using PacketQueue = ThreadSafeQueue<PacketPtr>;
  using ProtocolUniquePtr = std::unique_ptr<Protocol>;
  
  boost::asio::ip::tcp::socket mSocket;
  PacketQueue mSendingQueue;
  PacketPtr mSendingPacket;
  ByteBuffer mReceivingPacket;
  ProtocolUniquePtr mProtocol;
  Server& mServer;
  Player* mPlayer;
  ConnectionState mConnectionState;
  std::atomic_bool mSetCompressionIsSent;
  std::uint8_t mReceivingPacketSize[5];
  std::uint8_t mReceivingPacketCountSize;
  
  void handleRead(const boost::system::error_code& error, bool header = true);
  void readNext();
  void handleWrite(const boost::system::error_code& error);
  void writeNext();
};

using SessionUniquePtr = std::unique_ptr<Session>;

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