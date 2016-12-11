#ifndef REDI_SESSION
#define REDI_SESSION

#include "protocol/packet.hpp"
#include "threadsafequeue.hpp"

namespace redi
{

class Session
{
public:

  Session(boost::asio::ip::tcp::socket&& socket);

private:
  
  using PacketQueue = ThreadSafeQueue<protocol::Packet>;

  boost::asio::ip::tcp::socket mSocket;
  PacketQueue mPacketsToBeSend;
  PacketQueue mReceivedPackets;
};

} // namespace redi

#endif