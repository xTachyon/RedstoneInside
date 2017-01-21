#ifndef REDI_SESSION
#define REDI_SESSION

#include <atomic>
#include <boost/asio.hpp>
#include "threadsafequeue.hpp"
#include "sizeliteraloperators.hpp"
#include "bytebuffer.hpp"
#include "protocol/protocol.hpp"

namespace redi
{

enum class State
{
  Handshake,
  Play,
  Status,
  Login
};

inline const char* getStateName(State s)
{
  const char* ptr;
  
  switch (s)
  {
  case State::Play:
    ptr = "Play";
    break;
    
  case State::Status:
    ptr = "Status";
    break;
    
  case State::Login:
    ptr = "Login";
    break;
    
  case State::Handshake:
    ptr = "Handshake";
    break;
  }
  
  return ptr;
}

class Server;

class Session
{
public:

  State state;
  std::atomic_bool setCompressionSent;
  
  Session(boost::asio::ip::tcp::socket&& socket, Server* server);
  Session(Session&) = delete;
  Session(Session&& s);
  ~Session();
  
  void sendPacket(ByteBuffer&& pkt, const char* message);
  
  boost::asio::ip::tcp::socket& getSocket() { return mSocket; }
  Server& getServer() { return *mServer; }
  
  void kill();
  Protocol& getProtocol() { return *mProtocol; }
  Protocol* getProtocolPtr() { return mProtocol.get(); }
  void setPlayer(Player& player);
  boost::asio::io_service& getIoService() { return mSocket.get_io_service(); }

private:
  
  using PacketPtr = std::shared_ptr<ByteBuffer>;
  using PacketQueue = ThreadSafeQueue<PacketPtr>;
  using ProtocolPtr = std::unique_ptr<Protocol>;
  
  boost::asio::ip::tcp::socket mSocket;
  PacketQueue mSendingQueue;
  PacketPtr mSendingPacket;
  ByteBuffer mReceivingPacket;
  ProtocolPtr mProtocol;
  std::uint8_t mReceivingPacketSize[5];
  std::uint8_t mReceivingPacketCountSize;
  Server* mServer;
  Player* mPlayer;
  
  void handleRead(const boost::system::error_code& error, bool header = true);
  void readNext();
  void handleWrite(const boost::system::error_code& error);
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