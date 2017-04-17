#ifndef REDI_BASE64_HPP
#define REDI_BASE64_HPP

#include <string>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include "../bytebuffer.hpp"

namespace redi {
namespace util {

class Base64Encoder {
public:
  Base64Encoder() = delete;

  static ByteBuffer encode(const ByteBuffer& data) {
    return rawEncode<ByteBuffer, ByteBuffer>(data);
  }
  static ByteBuffer encode(const std::string& str) {
    return rawEncode<ByteBuffer, std::string>(str);
  }
  static std::string encodeToString(const ByteBuffer& data) {
    return rawEncode<std::string, ByteBuffer>(data);
  }
  static std::string encodeToString(const std::string& str) {
    return rawEncode<std::string, std::string>(str);
  }

private:
  template <typename T, typename K>
  static T rawEncode(const K& data) {
    namespace it = boost::archive::iterators;
    using encoder = it::base64_from_binary<
        it::transform_width<typename K::const_iterator, 6, 8>>;

    std::size_t pad = (3 - data.size() % 3) % 3;
    T result(encoder(data.begin()), encoder(data.end()));
    for (std::size_t i = 0; i < pad; ++i)
      result.push_back('=');

    return result;
  }
};

class Base64Decoder {
public:
  Base64Decoder() = delete;

  static ByteBuffer decode(const ByteBuffer& data) {
    return rawDecode<ByteBuffer, ByteBuffer>(data);
  }
  static ByteBuffer decode(const std::string& str) {
    return rawDecode<ByteBuffer, std::string>(str);
  }
  static std::string decodeToString(const ByteBuffer& data) {
    return rawDecode<std::string, ByteBuffer>(data);
  }
  static std::string decodeToString(const std::string& str) {
    return rawDecode<std::string, std::string>(str);
  }

private:
  template <typename T, typename K>
  static T rawDecode(const K& data) {
    namespace it = boost::archive::iterators;
    using decoder =
        it::transform_width<it::binary_from_base64<typename K::const_iterator>,
                            8, 6>;

    std::size_t pad = 0;
    if (data[data.size() - 1] == '=') {
      ++pad;
      if (data[data.size() - 2] == '=')
        ++pad;
    }

    T result(decoder(data.begin()), decoder(data.end()));
    result.erase(result.end() - pad, result.end());

    return result;
  }
};

} // namespace util
} // namespace redi

#endif // REDI_BASE64_HPP