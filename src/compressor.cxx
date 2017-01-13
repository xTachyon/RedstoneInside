#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include "compressor.hpp"

using namespace boost::iostreams;

namespace redi
{
namespace compressor
{

ByteBuffer compressZlib(const ByteBuffer& data, CompressionLevel level)
{
  ByteBuffer result;

  {
    filtering_ostream os;

    os.push(zlib_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

ByteBuffer decompressZlib(const ByteBuffer& data)
{
  ByteBuffer result;

  {
    filtering_ostream os;

    os.push(zlib_decompressor());
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

ByteBuffer compressGzip(const ByteBuffer& data, CompressionLevel level)
{
  ByteBuffer result;

  {
    filtering_ostream os;

    os.push(gzip_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

ByteBuffer decompressGzip(const ByteBuffer& data)
{
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