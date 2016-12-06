#ifndef REDI_CHUNK
#define REDI_CHUNK

#include <boost/multi_array.hpp>
#include "block.hpp"

namespace redi
{

class Chunk
{
public:

  Chunk();

private:

  using ChunkBlocks = boost::multi_array<Block, 3>;

  ChunkBlocks mBlocks;
};

} // namespace redi


#endif //REDI_CHUNK
