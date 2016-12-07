#include <boost/multi_array.hpp>
#include "chunk.hpp"

namespace redi
{

Chunk::Chunk() : mBlocks(boost::extents[16][256][16]) {}

} // namespace redi
