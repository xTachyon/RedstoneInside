#ifndef REDI_UTIL_STRING
#define REDI_UTIL_STRING

#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <typeinfo>

namespace redi
{
namespace util
{

#if defined __GNUC__ || defined __MINGW32__ || defined __MINGW64__
#include <cxxabi.h>

inline std::string demangleTypeName(const char* str)
{
  // https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
  int status;
  char* realname;
  
  realname = abi::__cxa_demangle(str, nullptr, 0, &status);
  std::string result(realname);
  free(realname);
  
  return result;
}
#else
inline std::string demangleTypeName(const char* str) { return str; }
#endif

template <typename T>
inline std::string demangleTypeName()
{
  return demangleTypeName(typeid(T).name());
}

template <typename T>
inline std::basic_string<T> toLowercase(std::basic_string<T> str)
{
  for (auto&  c : str)
  {
    c = std::tolower(c, std::locale());
  }
  
  return str;
}

template <typename T>
inline std::basic_string<T> toUppercase(std::basic_string<T> str)
{
  for (auto&  c : str)
  {
    c = std::toupper(c, std::locale());
  }
  
  return str;
}

template <typename T>
inline std::string toString(const T& ref)
{
  std::ostringstream ss;
  ss << ref;
  return ss.str();
}
  
template <typename From, typename To>
To binaryTo(const From& val)
{
  static_assert(sizeof(From) == sizeof(To), "sizeof(From) == sizeof(To)");
  To x;
  auto f = reinterpret_cast<const std::uint8_t*>(std::addressof(val));
  auto t = reinterpret_cast<std::uint8_t*>(std::addressof(x));
  std::copy(f, f + sizeof(val), t);
  
  return x;
}

} // namespace util
} // namespace redi

#endif // REDI_UTIL_STRING
