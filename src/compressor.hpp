#ifndef REDI_COMPRESSOR_HPP
# define REDI_COMPRESSOR_HPP

#include "bytebuffer.hpp"

namespace redi
{
namespace compressor
{

enum class CompressionLevel
{
  /*
  * Not a very good idea
  * but I don't want to include
  * boost here.
  */

  NoCompression = 0,
  Default = -1,
  BestSpeed = 1,
  BestCompression = 9
};

ByteBuffer compressZlib(const ByteBuffer& data, CompressionLevel level = CompressionLevel::BestCompression);
ByteBuffer decompressZlib(const ByteBuffer& data);

ByteBuffer compressGzip(const ByteBuffer& data, CompressionLevel level = CompressionLevel::BestCompression);
ByteBuffer decompressGzip(const ByteBuffer& data);

} // namespace compressor
} // namespace redi

#endif // REDI_COMPRESSOR_HPP