#pragma once

#include <string>
#include <boost/filesystem.hpp>

namespace redi {

class ServerIcon {
public:
  ServerIcon(const boost::filesystem::path& path = "icon.png");

  operator const std::string() const { return getData(); }
  operator bool() const { return loaded(); }

  void load(const boost::filesystem::path& path = "icon.png");

  const std::string& getData() const { return data; }
  bool loaded() const { return data.size() != 0; }

private:
  std::string data;
};

} // namespace redi