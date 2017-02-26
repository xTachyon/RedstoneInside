#include <boost/multi_array.hpp>
#include "chunk.hpp"

#include "../sizeliteraloperators.hpp"

namespace redi
{

Chunk::Chunk() : mBlocks(boost::extents[16][16]), mInhabitedTime(0)
{
}

} // namespace redi