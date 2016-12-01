#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include "compressor.hpp"

using namespace boost::iostreams;

namespace redi
{
namespace compressor
{

BinaryData compresszlib(const BinaryData& data, CompressionLevel level)
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

BinaryData decompresszlib(const BinaryData& data)
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

BinaryData compressgzip(const BinaryData& data, CompressionLevel level)
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

BinaryData decompresszgzip(const BinaryData& data)
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