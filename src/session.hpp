#ifndef REDI_SESSION
#define REDI_SESSION

#include <boost/asio.hpp>
#include "threadsafequeue.hpp"
#include "sizeliteraloperators.hpp"

namespace redi
{

enum class Stage
{
  Play,
  Status,
  Login
};

class Session
{
public:

  Stage stage;
  
  Session(boost::asio::ip::tcp::socket&& socket);
  Session(Session&& s);

private:
  
//  using PacketPtr = std::shared_ptr<protocol::Packet>;
//  using PacketQueue = ThreadSafeQueue<PacketPtr>;
//  static constexpr std::size_t MaximumTcpPackeSize = 64_KB;
//
//  boost::asio::ip::tcp::socket mSocket;
//  PacketQueue mPacketsToBeSend;
//  PacketPtr mSendingPacket;
//  ByteBuffer mReceivingPacket;
//
//  void handleRead(const boost::system::error_code& error, std::size_t bytes);
//  void readNext();
//  void writeNext();
//  void handleWrite(const boost::system::error_code& error);
};

} // namespace redi

#endif // REDI_SESSION