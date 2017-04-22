#ifndef REDI_UTIL_STRING
#define REDI_UTIL_STRING

#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <typeinfo>
#include "../datatypes.hpp"

namespace redi {
namespace util {

#if defined __GNUC__ || defined __MINGW32__ || defined __MINGW64__
//#include <cxxabi.h>

inline std::string demangleTypeName(const char* str) {
  // https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
  //  int status;
  //  char* realname;
  //
  //  realname = abi::__cxa_demangle(str, nullptr, 0, &status);
  //  std::string result(realname);
  //  free(realname);
  //
  //  return result;
  return str;
  // TODO: fix this
}
#else
inline std::string demangleTypeName(const char* str) { return str; }
#endif

template <typename T>
inline std::string demangleTypeName() {
  return demangleTypeName(typeid(T).name());
}

template <typename T>
inline std::basic_string<T> toLowercase(std::basic_string<T> str) {
  for (auto& c : str) {
    c = std::tolower(c, std::locale());
  }

  return str;
}

template <typename T>
inline std::basic_string<T> toUppercase(std::basic_string<T> str) {
  for (auto& c : str) {
    c = std::toupper(c, std::locale());
  }

  return str;
}

template <typename T>
inline std::string toString(const T& ref) {
  std::ostringstream ss;
  ss << ref;
  return ss.str();
}

template <typename T>
inline bool noCaseCompareEqual(const std::basic_string<T>& l,
                               const std::basic_string<T>& r) {
  if (l.size() != r.size())
    return false;

  for (std::size_t i = 0; i < l.size(); ++i) {
    if (std::tolower(l[i]) != std::tolower(r[i]))
      return false;
  }

  return true;
}

#ifdef _MSC_VER
template <>
inline bool noCaseCompareEqual<char>(const std::string& l,
                                     const std::string& r) {
  return _stricmp(l.c_str(), r.c_str()) == 0;
}
#endif

template <typename From, typename To>
To binaryTo(const From& val) {
  static_assert(sizeof(From) == sizeof(To), "sizeof(From) == sizeof(To)");
  To x;
  auto f = reinterpret_cast<const std::uint8_t*>(std::addressof(val));
  auto t = reinterpret_cast<std::uint8_t*>(std::addressof(x));
  std::copy(f, f + sizeof(val), t);

  return x;
}

template <typename T>
basic_string_view<T> trimleft(basic_string_view<T> str, basic_string_view<T> whitespaces = " \t\n\r") {
  str.remove_prefix(std::min(str.find_first_not_of(whitespaces), str.size()));
  return str;
}

//string_view trimleft(string_view str, string_view whitespaces = " \t\n\r") {
//  str.remove_prefix(std::min(str.find_first_not_of(whitespaces), str.size()));
//  return str;
//}

template <typename T>
basic_string_view<T> trimright(basic_string_view<T> str, basic_string_view<T> whitespaces = " \t\n\r") {
  auto foundat = str.find_last_not_of(whitespaces);
  str.remove_suffix(str.size() - std::min(foundat, str.size()) - 1);
  return str;
}

template <typename T>
basic_string_view<T> trim(basic_string_view<T> str, basic_string_view<T> whitespaces = " \t\n\r") {
  return trimright<T>(trimleft<T>(str, whitespaces), whitespaces);
}

} // namespace util
} // namespace redi

#endif // REDI_UTIL_STRING
