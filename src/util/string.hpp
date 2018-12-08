#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <typeinfo>
#include "../datatypes.hpp"

namespace redi {
namespace util {

std::string demangleTypeName(const char* str);

template <typename T>
inline std::string demangleTypeName() {
  return demangleTypeName(typeid(T).name());
}

template <typename T>
inline std::string demangleTypeName(T& type) {
  return demangleTypeName(typeid(type).name());
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

} // namespace util
} // namespace redi