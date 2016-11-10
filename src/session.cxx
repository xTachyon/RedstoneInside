#include <boost/asio.hpp>
#include "session.hpp"

namespace redi
{

Session::Session(boost::asio::ip::tcp::socket&& socket)
  : mSocket(std::move(socket)) {}

} // namespace redi