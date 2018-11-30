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
#include "lockfree/queue.hpp"
#include "hasserver.hpp"

namespace redi {

class Player;
class Server;
class Session;

using SessionUniquePtr = std::unique_ptr<Session>;
using SessionSharedPtr = std::shared_ptr<Session>;

class Session : public HasServer, public std::enable_shared_from_this<Session> {
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

  void sendPacket(ByteBuffer packet, const std::string& message = "");

  boost::asio::ip::tcp::socket& getSocket() { return socket; }
  const boost::asio::ip::tcp::socket& getSocket() const { return socket; }
  // And no, I don't have any idea why would I need a socket constant
  // Remember to add a reason here if you ever do this

  boost::asio::io_service& getIoService() { return socket.get_io_service(); }

  Player& getPlayer() { return *player; }
  void setPlayer(Player& player);

  ConnectionState getConnectionState() const { return connectionState; }
  void setConnectionState(ConnectionState s) { connectionState = s; }

  bool getCompressionIsSent() const { return setCompressionIsSentVar; }
  void setCompressionIsSent(bool b) { setCompressionIsSentVar = b; }

  void disconnect();
  void kick(const std::string& message);
  
  bool isDisconnecting() const { return isDisconnected; }
  
  void readNext();

private:
  using PacketPtr = std::shared_ptr<ByteBuffer>;
  using PacketQueue = ThreadSafeQueue<PacketPtr>;

  boost::asio::ip::tcp::socket socket;
  ByteBuffer sendingPacket;
  ByteBuffer receivingPacket;
  Player* player;
  ConnectionState connectionState;
  std::atomic_bool setCompressionIsSentVar;
  std::uint8_t receivingPacketSize[5];
  std::uint8_t receivingPacketCountSize;
  PacketHandlerSharedPtr packetHandler;
  std::atomic_bool isDisconnected;
  std::atomic_bool isWritting;
  lockfree::ByteBufferQueue packetsToBeSend;
  boost::asio::io_service::strand strand;
  
  void handleRead(const boost::system::error_code& error, bool header);
  void handleWrite(const boost::system::error_code& error);
  void writeNext();
  void postWrite();
};

inline bool operator==(const Session& l, const Session& r) { return &l == &r; }

inline bool operator!=(const Session& l, const Session& r) { return !(l == r); }

} // namespace redi

#endif // REDI_SESSION