#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include "util.hpp"

using namespace boost::asio;
using namespace boost::asio::ip;
namespace asio = boost::asio;

namespace redi
{
namespace util
{

std::int32_t getUnixTimestamp()
{
  boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
  return (boost::posix_time::second_clock::universal_time() - epoch).total_seconds();
}

std::vector<boost::asio::ip::tcp::resolver::iterator> resolve(const std::string& host, const std::string& port)
{
  std::vector<boost::asio::ip::tcp::resolver::iterator> result;

  asio::io_service io;
  asio::ip::tcp::resolver resolver(io);
  asio::ip::tcp::resolver::query query(host, port);
  asio::ip::tcp::resolver::iterator endpoint(resolver.resolve(query)), end;

  while (endpoint != end) result.push_back(endpoint++);

  return result;
}

#if defined __GNUC__ || defined __MINGW32__ || defined __MINGW64__
#include <cxxabi.h>

std::string demangleTypeName(const char* str)
{
  // https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
  int status;
  char* realname;

  realname = abi::__cxa_demangle(str, nullptr, 0, &status);
  std::string result(realname);
  free(realname);

  return result;
}
#endif

} // namespace util
} // namespace redi