#ifndef REDI_UTIL_UTIL
#define REDI_UTIL_UTIL

#include <string>
#include <typeinfo>
#include <vector>
#include <boost/asio.hpp>

namespace redi
{
namespace util
{

std::int32_t getUnixTimestamp();

std::vector<boost::asio::ip::tcp::resolver::iterator> resolve(const std::string& host, const std::string& port);

std::string demangleTypeName(const char* str);
#if !defined __GNUC__ && !defined __MINGW32__ && !defined __MINGW64__
inline std::string demangleTypeName(const char* str) { return str; }
#endif

template <typename T>
inline std::string demangleTypeName()
{
  return demangleTypeName(typeid(T).name());
}

} // namespace util
} // namespace redi

#endif // REDI_UTIL_UTIL