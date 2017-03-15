#include <boost/multi_array.hpp>
#include "chunk.hpp"

#include "../sizeliteraloperators.hpp"

namespace redi
{

Chunk::Chunk() : blocks(boost::extents[16][16])
{
}

} // namespace redi