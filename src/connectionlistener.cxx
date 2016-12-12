#include "connectionlistener.hpp"

namespace redi
{
  
ConnectionListener::ConnectionListener(boost::asio::io_service& io)
      : mIoService(io), mSocket(mIoService)
{
  
}

} // namespace redi
