#ifndef REDI_CHUNK
#define REDI_CHUNK

#include <cstdint>
#include <boost/multi_array.hpp>
#include <boost/optional.hpp>
#include "block.hpp"
#include "bytebuffer.hpp"
#include "nbt/nbt.hpp"
#include "position.hpp"

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
  Chunk(const ByteBuffer& data) : Chunk() { *this = data; }
  Chunk(const nbt::RootTag& root) : Chunk() { *this = root; }
  
  Chunk& operator=(const ByteBuffer& data);
  Chunk& operator=(const nbt::RootTag& root);
  
  Block& operator()(BlockPosition pos) { return mBlocks[pos.x][pos.y][pos.z]; };
  Block& operator()(std::int32_t x, std::int32_t y, std::int32_t z) { return mBlocks[x][y][z]; }

private:

  using ChunkBlocks = boost::multi_array<Block, 3>;

  ChunkBlocks mBlocks;
  std::uint64_t mInhabitedTime;
  
  void deserializeChunk(const nbt::RootTag& root);
  void deserializeBiomes(const nbt::TagIntArray& biomes) = delete;
  void deserializeSections(const nbt::TagList& sections);
  void deserializeSection(std::int8_t nthSection, const nbt::Int8Vector& blocks,
                          boost::optional<const nbt::Int8Vector&> add,
                          const nbt::Int8Vector& data, const nbt::Int8Vector& blocklight,
                          const nbt::Int8Vector& skylight);
};

void bestTerrainGenerator(Chunk& ch);

} // namespace redi


#endif //REDI_CHUNK
