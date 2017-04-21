#include "../buffers.hpp"

#ifndef REDI_UTIL_ZIP_HPP
#define REDI_COMPRESSOR_HPP

namespace redi::util::zip {

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

namespace zlib {

void compress(ConstBuffer input, ByteBuffer& output, CompressionLevel level = CompressionLevel::BestCompression);

ByteBuffer compress(ConstBuffer input, CompressionLevel level = CompressionLevel::BestCompression);

void decompress(ConstBuffer input, ByteBuffer& output);

ByteBuffer decompress(ConstBuffer input);

}

namespace gzip {

void compress(ConstBuffer input, ByteBuffer& output, CompressionLevel level = CompressionLevel::BestCompression);

ByteBuffer compress(ConstBuffer input, CompressionLevel level = CompressionLevel::BestCompression);

void decompress(ConstBuffer input, ByteBuffer& output);

ByteBuffer decompress(ConstBuffer input);

}

} // namespace redi::util::zip

#endif // REDI_COMPRESSOR_HPP