#pragma once

namespace redi {

class Chunk;

class TerrainGenerator {
public:
  virtual ~TerrainGenerator() = default;

  void generate(Chunk& chunk);
};

using WorldGenerator = std::shared_ptr<TerrainGenerator>;

} // namespace redi