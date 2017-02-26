#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include "connectionlistener.hpp"
#include "logger.hpp"
#include "server.hpp"

namespace asio = boost::asio;

namespace redi
{

ConnectionListener::ConnectionListener(asio::io_service& io, std::uint16_t port, Server& server)
      : mIoService(io), mSocket(mIoService),
        mAcceptor(mIoService, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
        mServer(server), mIsStopping(false)
{
  Logger::debug((boost::format("ConnectionListener %1% created") % this).str());
}

ConnectionListener::~ConnectionListener()
{
  Logger::debug((boost::format("ConnectionListener %1% destroyed") % this).str());
}

void ConnectionListener::listen()
{
  mAcceptor.async_accept(mSocket,
                        boost::bind(&ConnectionListener::handleAccept, shared_from_this(),
                                    asio::placeholders::error));
}

void ConnectionListener::handleAccept(const boost::system::error_code& error)
{
  if (error)
  {
    Logger::error(error.message());
  }
  else if (!mIsStopping)
  {
    mServer.addConnectedSession(std::move(mSocket));
    listen();
  }
}
  
} // namespace redi
