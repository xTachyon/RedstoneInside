#ifndef REDI_CONNECTIONLISTENER
#define REDI_CONNECTIONLISTENER

#include <thread>
#include <boost/asio.hpp>

namespace redi
{

class Server;

class ConnectionListener
{
public:
  
  ConnectionListener(boost::asio::io_service& io, std::uint16_t port, Server* ptr);
  ConnectionListener(const ConnectionListener&) = delete;
  ~ConnectionListener();
  
  ConnectionListener& operator=(const ConnectionListener&) = delete;
  
private:
  
  boost::asio::io_service& mIoService;
  boost::asio::ip::tcp::socket mSocket;
  boost::asio::ip::tcp::acceptor mAcceptor;
  std::thread mThread;
  Server* mServer;
  
  void serverThread();
  void listen();
  void handleAccept(const boost::system::error_code& error);
};
  
} // namespace redi

#endif // REDI_CONNECTIONLISTENER