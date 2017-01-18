#ifndef REDI_CHUNK
#define REDI_CHUNK

#include <cstdint>
#include <boost/multi_array.hpp>
#include <boost/optional.hpp>
#include "block.hpp"
#include "../bytebuffer.hpp"
#include "../nbt/nbt.hpp"
#include "../vectorn.hpp"
#include "chunkcolumn.hpp"
#include "terraingenerator.hpp"

namespace redi
{

class Chunk
{
public:

  static constexpr std::uint16_t ChunkMaxX = 16;
  static constexpr std::uint16_t ChunkMaxY = 256;
  static constexpr std::uint16_t ChunkMaxZ = 16;
  static constexpr std::int8_t BlocksPerSection = 16;

  Chunk();

  Block& operator()(Vector3i pos) { return mBlocks[pos.x][pos.z][pos.y]; };
  Block& operator()(std::int32_t x, std::int32_t y, std::int32_t z) { return mBlocks[x][z][y]; }
  Block operator()(Vector3i pos) const { return mBlocks[pos.x][pos.z][pos.y]; };
  Block operator()(std::int32_t x, std::int32_t y, std::int32_t z) const { return mBlocks[x][z][y]; }

private:

  using ChunkBlocks = boost::multi_array<ChunkColumn, 2>;

  ChunkBlocks mBlocks;
  std::uint64_t mInhabitedTime;
  std::unique_ptr<TerrainGenerator> mGenerator;
};

} // namespace redi


#endif //REDI_CHUNK
