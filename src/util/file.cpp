#include <fstream>
#include "util.hpp"
#include "../filesystem.hpp"

namespace redi {
namespace util {

ByteBuffer readFile(const std::string& path) {
  if (!fs::exists(path)) {
    throw std::runtime_error(path + " doesn't exist");
  }

  ByteBuffer buffer;
  auto size = fs::file_size(path);
  buffer.resize(static_cast<ByteBuffer::size_type>(size));
  
  std::ifstream file(path, std::ios::binary);
  file.read(buffer.as_char(), size);

  return buffer;
}

std::string readFileToString(const std::string& path) {
  if (!fs::exists(path)) {
    throw std::runtime_error(path + " doesn't exist");
  }

  std::string str;
  auto size = fs::file_size(path);
  str.resize(static_cast<ByteBuffer::size_type>(size));
  
  std::ifstream file(path, std::ios::binary);
  file.read(&str[0], size);

  return str;
}

} // namespace util
} // namespace redi
