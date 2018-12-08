#pragma once

#include <atomic>
#include <thread>
#include <boost/asio.hpp>
#include "hasserver.hpp"

namespace redi {

class ConnectionListener;
class Server;

using ConnectionListenerSharedPtr = std::shared_ptr<ConnectionListener>;

class ConnectionListener
    : public HasServer, public std::enable_shared_from_this<ConnectionListener> {
public:
  ConnectionListener(boost::asio::io_service& io, std::uint16_t port,
                     Server& server);
  ConnectionListener(const ConnectionListener&) = delete;
  ~ConnectionListener();

  ConnectionListener& operator=(const ConnectionListener&) = delete;

private:
  friend class Server;

  boost::asio::io_service& mIoService;
  boost::asio::ip::tcp::socket mSocket;
  boost::asio::ip::tcp::acceptor mAcceptor;
  std::atomic_bool mIsStopping;

  void listen();
  void handleAccept(const boost::system::error_code& error);
};

} // namespace redi