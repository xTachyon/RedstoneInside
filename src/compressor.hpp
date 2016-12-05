#ifndef REDI_COMPRESSOR_HPP
# define REDI_COMPRESSOR_HPP

#include "binarydata.hpp"

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

BinaryData compressZlib(const BinaryData& data, CompressionLevel level = CompressionLevel::BestCompression);
BinaryData decompressZlib(const BinaryData& data);

BinaryData compressGzip(const BinaryData& data, CompressionLevel level = CompressionLevel::BestCompression);
BinaryData decompressGzip(const BinaryData& data);

} // namespace compressor
} // namespace redi

#endif // REDI_COMPRESSOR_HPP