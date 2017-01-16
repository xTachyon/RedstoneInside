#ifndef REDI_WORLD_CHUNKCOLUMN_HPP
#define REDI_WORLD_CHUNKCOLUMN_HPP

#include <array>
#include <cstdint>
#include "block.hpp"
#include "biome.hpp"

namespace redi
{

struct ChunkColumn
{
  static constexpr std::uint16_t BlocksPerColumn = 256;
  
  std::array<Block, BlocksPerColumn> blocks;
  Biome biome;
  
  ChunkColumn(Biome b = Biome::Ocean) : biome(b) {}
  
  Block& operator[](std::int32_t y) { return blocks[y]; }
};

  
} // namespace redi

#endif // REDI_WORLD_CHUNKCOLUMN_HPP
