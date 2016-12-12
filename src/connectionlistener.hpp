#ifndef REDI_CONNECTIONLISTENER
#define REDI_CONNECTIONLISTENER

#include <thread>
#include <boost/asio.hpp>

namespace redi
{

class ConnectionListener
{
public:
  
  ConnectionListener(boost::asio::io_service& io);
  
private:
  
  boost::asio::io_service& mIoService;
  boost::asio::ip::tcp::socket mSocket;
  std::thread mThread;
};
  
} // namespace redi


#endif // REDI_CONNECTIONLISTENER