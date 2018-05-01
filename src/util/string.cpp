#include "string.hpp"

#if defined __GNUC__ || defined __MINGW32__ || defined __MINGW64__
#include <cxxabi.h>
#endif

namespace redi::util {

#if defined __GNUC__ || defined __MINGW32__ || defined __MINGW64__
std::string demangleTypeName(const char* str) {
  // https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html
    int status;
    char* realname;

    realname = abi::__cxa_demangle(str, nullptr, 0, &status);
    std::string result(realname);
    free(realname);

    return result;
}
#else
std::string demangleTypeName(const char* str) {
  return str;
}
#endif

}