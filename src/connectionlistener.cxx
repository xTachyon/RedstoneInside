#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "connectionlistener.hpp"
#include "logger.hpp"
#include "server.hpp"

namespace asio = boost::asio;

namespace redi
{

ConnectionListener::ConnectionListener(asio::io_service& io, std::uint16_t port, Server* ptr)
      : mIoService(io), mSocket(mIoService),
        mAcceptor(mIoService, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
        mServer(ptr)
{
  mThread = std::thread(&ConnectionListener::serverThread, this);
}

ConnectionListener::~ConnectionListener()
{
  mIoService.stop();
  mThread.join();
}

void ConnectionListener::listen()
{
  mAcceptor.async_accept(mSocket,
                        boost::bind(&ConnectionListener::handleAccept, this,
                                    asio::placeholders::error));
}

void ConnectionListener::handleAccept(const boost::system::error_code& error)
{
  if (error)
  {
    Logger::error(error.message());
  }
  else
  {
    if (mServer->getAcceptConnections()) mServer->addConnectedSession(std::move(mSocket));
    else mSocket.close();
  }
  
  listen();
}

void ConnectionListener::serverThread()
{
  listen();
  mIoService.run();
}
  
} // namespace redi
