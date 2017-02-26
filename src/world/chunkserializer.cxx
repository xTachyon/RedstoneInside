#include "chunkserializer.hpp"

namespace redi
{
namespace world
{

ChunkSerializer::ChunkSerializer(nbt::RootTag& root, const Chunk& chunk)
      : chunk(chunk), root(root) {}

void ChunkSerializer::operator()()
{
  
}
  
} // namespace world
} // namespace redi
