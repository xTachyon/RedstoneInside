#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "connectionlistener.hpp"

namespace asio = boost::asio;

namespace redi
{

ConnectionListener::ConnectionListener(asio::io_service& io, std::uint16_t port)
      : mIoService(io), mSocket(mIoService),
        mAcceptor(mIoService, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
  mThread = std::thread(&ConnectionListener::serverThread, this);
}

ConnectionListener::~ConnectionListener()
{
  mIoService.stop();
  mThread.join();
}

void ConnectionListener::listen(asio::ip::tcp::socket& socket, boost::asio::ip::tcp::acceptor& acceptor)
{
  acceptor.async_accept(socket,
                        boost::bind(ConnectionListener::handleAccept,
                                    asio::placeholders::error,
                                    std::ref(socket),
                                    std::ref(acceptor)));
}

void ConnectionListener::handleAccept(const boost::system::error_code& error, asio::ip::tcp::socket& socket,
                                      asio::ip::tcp::acceptor& acceptor)
{
  if (!error)
  {
    socket.close();
  }
  
  ConnectionListener::listen(socket, acceptor);
}

void ConnectionListener::serverThread()
{
  listen(mSocket, mAcceptor);
  mIoService.run();
}
  
} // namespace redi
