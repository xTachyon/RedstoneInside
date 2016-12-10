#ifndef REDI_CHUNK
#define REDI_CHUNK

#include <cstdint>
#include <boost/multi_array.hpp>
#include <boost/optional.hpp>
#include "block.hpp"
#include "binarydata.hpp"
#include "nbt/nbt.hpp"

namespace redi
{

class Chunk
{
public:

  static constexpr std::size_t ChunkMaxX = 16;
  static constexpr std::size_t ChunkMaxY = 256;
  static constexpr std::size_t ChunkMaxZ = 16;
  
  Chunk();
  Chunk(const BinaryData& data) { *this = data; }
  Chunk(const nbt::RootTag& root) { *this = root; }
  
  Chunk& operator=(const BinaryData& data);
  Chunk& operator=(const nbt::RootTag& root);

private:

  using ChunkBlocks = boost::multi_array<Block, 3>;

  ChunkBlocks mBlocks;
  std::uint64_t mInhabitedTime;
  
  void deserializeChunk(const nbt::RootTag& root);
  void deserializeBiomes(const nbt::TagIntArray& biomes) = delete;
  void deserializeSections(const nbt::TagList& sections);
  void deserializeSection(std::int8_t nthSection, const nbt::TagByteArray& blocks,
                          boost::optional<const nbt::TagByteArray&> add,
                          const nbt::TagByteArray& data, const nbt::TagByteArray& blocklight,
                          const nbt::TagByteArray& skylight);
};

} // namespace redi


#endif //REDI_CHUNK
