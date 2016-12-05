#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include "compressor.hpp"

using namespace boost::iostreams;

namespace redi
{
namespace compressor
{

BinaryData compressZlib(const BinaryData& data, CompressionLevel level)
{
  BinaryData result;

  {
    filtering_ostream os;

    os.push(zlib_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

BinaryData decompressZlib(const BinaryData& data)
{
  BinaryData result;

  {
    filtering_ostream os;

    os.push(zlib_decompressor());
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

BinaryData compressGzip(const BinaryData& data, CompressionLevel level)
{
  BinaryData result;

  {
    filtering_ostream os;

    os.push(gzip_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(result));

    write(os, reinterpret_cast<const char*>(&data[0]), data.size());
  }

  return result;
}

BinaryData decompressGzip(const BinaryData& data)
{
  BinaryData result;

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