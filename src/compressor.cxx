#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include "compressor.hpp"

using namespace boost::iostreams;

namespace redi {
namespace compressor {

ByteBuffer compressZlib(const ByteBuffer& data, CompressionLevel level) {
  ByteBuffer result;

  {
    filtering_ostream os;

    os.push(zlib_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

void compressZlib(const ByteBuffer& in, ByteBuffer& out, CompressionLevel level,
                  std::size_t start, std::size_t len) {
  std::size_t size;
  if (len == std::numeric_limits<std::size_t>::max()) {
    len = in.size() - start;
  }

  {
    filtering_ostream os;

    os.push(zlib_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(out));

    write(os, in.as_const_char() + start, len);
  }
}

ByteBuffer decompressZlib(const ByteBuffer& data) {
  ByteBuffer result;

  {
    filtering_ostream os;

    os.push(zlib_decompressor());
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

ByteBuffer decompressZlib(const ByteBuffer& data, std::size_t start,
                          std::size_t len) {
  ByteBuffer result;
  if (len == std::numeric_limits<std::size_t>::max()) {
    len = data.size() - start;
  }

  {
    filtering_ostream os;

    os.push(zlib_decompressor());
    os.push(std::back_inserter(result));

    write(os, data.as_const_char() + start, len);
  }

  return result;
}

ByteBuffer compressGzip(const ByteBuffer& data, CompressionLevel level) {
  ByteBuffer result;

  {
    filtering_ostream os;

    os.push(gzip_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

ByteBuffer decompressGzip(const ByteBuffer& data) {
  ByteBuffer result;

  {
    filtering_ostream os;

    os.push(gzip_decompressor());
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

} // namespace compressor
} // namespace redi