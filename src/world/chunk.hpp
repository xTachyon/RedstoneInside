#pragma once

#include <cstdint>
#include <boost/multi_array.hpp>
#include <boost/optional.hpp>
#include "block.hpp"
#include "../bytebuffer.hpp"
#include "../vectorn.hpp"
#include "chunkcolumn.hpp"
#include "terraingenerator.hpp"

namespace redi {

namespace world {
struct ChunkDeserializer;
}

class Chunk {
public:
  using ChunkColumns = boost::multi_array<ChunkColumn, 2>;

  static constexpr std::uint16_t ChunkMaxX = 16;
  static constexpr std::uint16_t ChunkMaxY = 256;
  static constexpr std::uint16_t ChunkMaxZ = 16;

  std::int64_t inhabitedTime{};
  Vector2i position;

  Chunk();

  Block& operator()(Vector3i pos) { return blocks[pos.x][pos.z][pos.y]; };
  Block& operator()(std::int32_t x, std::int32_t y, std::int32_t z) {
    return blocks[x][z][y];
  }
  Block operator()(Vector3i pos) const { return blocks[pos.x][pos.z][pos.y]; };
  Block operator()(std::int32_t x, std::int32_t y, std::int32_t z) const {
    return blocks[x][z][y];
  }

  const ChunkColumns& getChunkColumns() const { return blocks; }

private:
  friend struct world::ChunkDeserializer;

  ChunkColumns blocks;
};

using ChunkUniquePtr = std::unique_ptr<Chunk>;

} // namespace redi