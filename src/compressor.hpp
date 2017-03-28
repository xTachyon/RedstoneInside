#ifndef REDI_COMPRESSOR_HPP
#define REDI_COMPRESSOR_HPP

#include <limits>
#include "bytebuffer.hpp"

namespace redi {
namespace compressor {

enum class CompressionLevel {
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

ByteBuffer
compressZlib(const ByteBuffer& data,
             CompressionLevel level = CompressionLevel::BestCompression);
void compressZlib(const ByteBuffer& in, ByteBuffer& out,
                  CompressionLevel level = CompressionLevel::BestCompression,
                  std::size_t start = 0,
                  std::size_t len = std::numeric_limits<std::size_t>::max());
// ByteBuffer decompressZlib(const ByteBuffer& data);
ByteBuffer
decompressZlib(const ByteBuffer& data, std::size_t start = 0,
               std::size_t len = std::numeric_limits<std::size_t>::max());

ByteBuffer
compressGzip(const ByteBuffer& data,
             CompressionLevel level = CompressionLevel::BestCompression);
ByteBuffer decompressGzip(const ByteBuffer& data);

} // namespace compressor
} // namespace redi

#endif // REDI_COMPRESSOR_HPP