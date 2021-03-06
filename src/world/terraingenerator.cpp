#include <cstdint>
#include "chunk.hpp"
#include "terraingenerator.hpp"
#include "../util/util.hpp"

namespace redi {

void TerrainGenerator::generate(Chunk& chunk) {
  for (std::int32_t x = 0; x < Chunk::ChunkMaxX; ++x) {
    for (std::int32_t y = 0; y < Chunk::ChunkMaxY; ++y) {
      for (std::int32_t z = 0; z < Chunk::ChunkMaxZ; ++z) {
        if (y == 0) {
          chunk(x, y, z).type = BlockType::Bedrock;
        } else if (y < 2) {
          chunk(x, y, z).type = BlockType::DiamondBlock;
        }
      }
    }
  }
}

} // namespace redi