#ifndef REDI_CONNECTIONLISTENER
#define REDI_CONNECTIONLISTENER

#include <thread>
#include <boost/asio.hpp>

namespace redi
{

class ConnectionListener
{
public:
  
  ConnectionListener(boost::asio::io_service& io, std::uint16_t port);
  ConnectionListener(const ConnectionListener&) = delete;
  ~ConnectionListener();
  
  ConnectionListener& operator=(const ConnectionListener&) = delete;
  
  // TODO: do this with delegates
  
  static void listen(boost::asio::ip::tcp::socket& socket, boost::asio::ip::tcp::acceptor& acceptor);
  static void handleAccept(const boost::system::error_code& error, boost::asio::ip::tcp::socket& socket,
                           boost::asio::ip::tcp::acceptor& acceptor);
  
private:
  
  boost::asio::io_service& mIoService;
  boost::asio::ip::tcp::socket mSocket;
  boost::asio::ip::tcp::acceptor mAcceptor;
  std::thread mThread;
  
  void serverThread();
};
  
} // namespace redi

#endif // REDI_CONNECTIONLISTENER