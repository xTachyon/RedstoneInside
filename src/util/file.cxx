#include <fstream>
#include <boost/filesystem.hpp>
#include "util.hpp"

namespace fs = boost::filesystem;

namespace redi {
namespace util {

ByteBuffer readFile(const std::string& path) {
  if (!fs::exists(path))
    throw std::runtime_error(path + " doesn't exist");

  ByteBuffer buffer;
  auto size = fs::file_size(path);
  buffer.resize(static_cast<std::size_t>(size));

  std::ifstream file(path);
  file.read(buffer.as_char(), size);

  return buffer;
}

std::string readFileToString(const std::string& path) {
  if (!fs::exists(path))
    throw std::runtime_error(path + " doesn't exist");

  std::string str;
  auto size = fs::file_size(path);
  str.resize(static_cast<std::size_t>(size));

  std::ifstream file(path);
  file.read(&str[0], size);

  return str;
}

} // namespace util
} // namespace redi
