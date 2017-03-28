#ifndef REDI_UTIL_FILE_HPP
#define REDI_UTIL_FILE_HPP

#include <string>
#include "../bytebuffer.hpp"

namespace redi {
namespace util {

ByteBuffer readFile(const std::string& path);
std::string readFileToString(const std::string& path);

} // namespace util
} // namespace redi

#endif // REDI_UTIL_FILE_HPP