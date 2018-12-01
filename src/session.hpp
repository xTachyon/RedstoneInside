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
#include "socket.hpp"

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

  Session(SocketSharedPtr socket, Server& server);
  Session(const Session&) = delete;
  Session(Session&& s) = delete;
  ~Session() override;

  Session& operator=(const Session&) = delete;
  Session& operator=(Session&&) = delete;

  void sendPacket(ByteBuffer packet, const std::string& message = "");

  Player& getPlayer() { return *player; }
  void setPlayer(Player& player);

  ConnectionState getConnectionState() const { return connectionState; }
  void setConnectionState(ConnectionState s) { connectionState = s; }

  bool getCompressionIsSent() const { return setCompressionIsSentVar; }
  void setCompressionIsSent(bool b) { setCompressionIsSentVar = b; }

  void disconnect();
  void kick(const std::string& message);
  
  bool isDisconnecting() const { return isDisconnected; }
  
private:
  using PacketPtr = std::shared_ptr<ByteBuffer>;
  using PacketQueue = ThreadSafeQueue<PacketPtr>;

  SocketSharedPtr socket;
  Player* player;
  ConnectionState connectionState;
  std::atomic_bool setCompressionIsSentVar;
  PacketHandlerSharedPtr packetHandler;
  std::atomic_bool isDisconnected;
  std::atomic_bool isWritting;
  ByteBuffer reading_buffer_vector;
  byte reading_buffer[4096];

  void on_read(size_t bytes, std::string error);
  void deserialize_packets();
};

inline bool operator==(const Session& l, const Session& r) { return &l == &r; }

inline bool operator!=(const Session& l, const Session& r) { return !(l == r); }

} // namespace redi

#endif // REDI_SESSION