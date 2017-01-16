#include <cstdint>
#include "chunk.hpp"
#include "terraingenerator.hpp"

namespace redi
{

void TerrainGenerator::generate(Chunk& chunk)
{
  for (std::int32_t x = 0; x < Chunk::ChunkMaxX; ++x)
  {
    for (std::int32_t z = 0; z < Chunk::ChunkMaxZ; ++z)
    {
      chunk(x, 0, z).type = BlockType::Bedrock;
      chunk(x, 1, z).type = BlockType::Glowstone;
      chunk(x, 2, z).type = BlockType::Leaves2;
    }
  }
}

} // namespace redi