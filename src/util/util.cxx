#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include "util.hpp"

using namespace boost::posix_time;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace redi
{
namespace util
{

//Int getUnixTimestamp()
//{
//  ptime epoch(boost::gregorian::date(1970, 1, 1));
//  return (second_clock::universal_time() - epoch).total_seconds();
//}
//
//std::vector<boost::asio::ip::tcp::resolver::iterator> resolve(const String& host, const String& port)
//{
//  std::vector<boost::asio::ip::tcp::resolver::iterator> result;
//
//  io_service io;
//  tcp::resolver resolver(io);
//  tcp::resolver::query query(host, port);
//  tcp::resolver::iterator endpoint(resolver.resolve(query)), end;
//
//  while (endpoint != end) result.push_back(endpoint++);
//
//  return result;
//}

} // namespace util
} // namespace redi