#include "servericon.hpp"
#include "bytebuffer.hpp"
#include "util/util.hpp"

namespace fs = boost::filesystem;

namespace redi {

ServerIcon::ServerIcon(const boost::filesystem::path& path) {
  load(path);
}

void ServerIcon::load(const boost::filesystem::path& path) {
  data.clear();
  
  if (!fs::exists(path) || fs::is_directory(path)) {
    return;
  }
  
  ByteBuffer buffer;
  
  {
    std::ifstream in(path.string(), std::ios::binary);
    if (!in) {
      return;
    }
    
    auto size = fs::file_size(path);
    
    buffer.resize(size);
    in.read(buffer.as_char(), size);
  }
  
  data = "data:image/png;base64," + util::Base64Encoder::encodeToString(buffer);
}

} // namespace redi