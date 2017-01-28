#include <cstdint>
#include "chunk.hpp"
#include "terraingenerator.hpp"

namespace redi
{

void TerrainGenerator::generate(Chunk& chunk)
{
  for (std::int32_t x = 0; x < Chunk::ChunkMaxX; ++x)
  {
    for (std::int32_t y = 0; y < Chunk::ChunkMaxY; ++y)
    {
      for (std::int32_t z = 0; z < Chunk::ChunkMaxZ; ++z)
      {
        if (y == 0)
        {
          chunk(x, y, z).type = BlockType::Bedrock;
        }
        else if (y == 1)
        {
          chunk(x, y, z).type = BlockType::Glowstone;
        }
        else if (y == 2)
        {
          chunk(x, y, z).type = BlockType::Leaves2;
        }
        else if (y == 30)
        {
          chunk(x, y, z).type = BlockType::EmeraldBlock;
        }
      }
        
    }
  }
}

} // namespace redi