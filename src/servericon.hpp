#ifndef REDI_SERVERICON_HPP
#define REDI_SERVERICON_HPP

#include <string>
#include <boost/filesystem.hpp>

namespace redi {

class ServerIcon {
public:
  static constexpr char const* DefaultImageLocation = "icon.png";
  
  ServerIcon(const boost::filesystem::path& path = DefaultImageLocation);
  
  operator const std::string() const { return getData(); }
  
  operator bool() const { return loaded(); }
  
  void load(const boost::filesystem::path& path = DefaultImageLocation);
  
  const std::string& getData() const { return data; }
  
  bool loaded() const { return data.size() != 0; }

private:
  std::string data;
};

} // namespace redi

#endif // REDI_SERVERICON_HPP