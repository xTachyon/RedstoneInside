#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include "compressor.hpp"

using namespace boost::iostreams;

namespace redi::util::zip {

namespace zlib {

void compress(ConstBuffer input, ByteBuffer& output, CompressionLevel level) {
  {
    boost::iostreams::filtering_ostream os;
    
    os.push(boost::iostreams::zlib_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(output));
    
    boost::iostreams::write(os, input.asConstChar(), input.size());
  }
}

ByteBuffer compress(ConstBuffer input, CompressionLevel level) {
  ByteBuffer output;
  compress(input, output, level);
  return output;
}

void decompress(ConstBuffer input, ByteBuffer& output) {
  {
    filtering_ostream os;
    
    os.push(zlib_decompressor());
    os.push(std::back_inserter(output));
    
    write(os, input.asConstChar(), input.size());
  }
}

ByteBuffer decompress(ConstBuffer input) {
  ByteBuffer output;
  decompress(input, output);
  return output;
}

}

namespace gzip {

void compress(ConstBuffer input, ByteBuffer& output, CompressionLevel level) {
  {
    boost::iostreams::filtering_ostream os;
    
    os.push(boost::iostreams::gzip_compressor(static_cast<int>(level)));
    os.push(std::back_inserter(output));
    
    boost::iostreams::write(os, input.asConstChar(), input.size());
  }
}

ByteBuffer compress(ConstBuffer input, CompressionLevel level) {
  ByteBuffer output;
  compress(input, output, level);
  return output;
}

void decompress(ConstBuffer input, ByteBuffer& output) {
  {
    filtering_ostream os;
    
    os.push(gzip_decompressor());
    os.push(std::back_inserter(output));
    
    write(os, input.asConstChar(), input.size());
  }
}

ByteBuffer decompress(ConstBuffer input) {
  ByteBuffer output;
  decompress(input, output);
  return output;
}

}

} // namespace redi::util::zip