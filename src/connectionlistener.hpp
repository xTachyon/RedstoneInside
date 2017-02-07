#ifndef REDI_CONNECTIONLISTENER
#define REDI_CONNECTIONLISTENER

#include <thread>
#include <boost/asio.hpp>

namespace redi
{

class ConnectionListener;
class Server;

using ConnectionListenerSharedPtr = std::shared_ptr<ConnectionListener>;

class ConnectionListener : public std::enable_shared_from_this<ConnectionListener>
{
public:
  
  ConnectionListener(boost::asio::io_service& io, std::uint16_t port, Server& server);
  ConnectionListener(const ConnectionListener&) = delete;
  ~ConnectionListener();
  
  ConnectionListener& operator=(const ConnectionListener&) = delete;
  
  Server& getServer() { return mServer; }
  
private:
  
  friend class Server;
  
  boost::asio::io_service& mIoService;
  boost::asio::ip::tcp::socket mSocket;
  boost::asio::ip::tcp::acceptor mAcceptor;
  Server& mServer;
  std::atomic_bool mIsStopping;
  
  void listen();
  void handleAccept(const boost::system::error_code& error);
};

  
} // namespace redi

#endif // REDI_CONNECTIONLISTENER