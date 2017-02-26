#include "chunkdeseriazer.hpp"

namespace redi
{
namespace world
{

ChunkDeserializer::ChunkDeserializer(const Chunk& chunk, nbt::RootTag& root)
      : chunk(chunk), root(root) {}

void ChunkDeserializer::operator()()
{
  
}
  
} // namespace world
} // namespace redi