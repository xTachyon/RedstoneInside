#ifndef REDI_SESSION
# define REDI_SESSION

# include <deque>
# include "protocol/packet.hpp"

namespace redi
{

class Session
{
public:

  Session(boost::asio::ip::tcp::socket&& socket);

private:

  boost::asio::ip::tcp::socket mSocket;
  std::deque<std::shared_ptr<protocol::Packet>> mToBeSend;
};

} // namespace redi

#endif