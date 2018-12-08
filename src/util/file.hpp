#pragma once

#include <string>
#include "../bytebuffer.hpp"

namespace redi {
namespace util {

ByteBuffer readFile(const std::string& path);
std::string readFileToString(const std::string& path);

} // namespace util
} // namespace redi