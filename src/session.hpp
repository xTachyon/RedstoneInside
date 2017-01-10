#ifndef REDI_SESSION
#define REDI_SESSION

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
    ptr = "Status;";
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

  State stage;
  
  Session(boost::asio::ip::tcp::socket&& socket, Server* server);
  Session(Session&) = delete;
  Session(Session&& s);
  
  void sendPacket(ByteBuffer&& pkt)
  {
    mSendingQueue.push(std::make_shared<ByteBuffer>(std::move(pkt)));
    writeNext();
  }
  
  boost::asio::ip::tcp::socket& getSocket() { return mSocket; }
  Server& getServer() { return *mServer; }
  
  void kill();
  void setProtocol(SessionPtr ptr);
  Protocol& getProtocol() { return *mProtocol; }

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
  
  void handleRead(const boost::system::error_code& error, bool header = true);
  void readNext();
  void handleWrite(const boost::system::error_code& error);
  void writeNext();
};

using SessionPtr = std::shared_ptr<Session>;

} // namespace redi

#endif // REDI_SESSION