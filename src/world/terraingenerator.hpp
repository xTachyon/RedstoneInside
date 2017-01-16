#ifndef REDI_WORLD_TERRAINGENERATOR_HPP
#define REDI_WORLD_TERRAINGENERATOR_HPP

namespace redi
{

class Chunk;

class TerrainGenerator
{
  public:
  
  virtual ~TerrainGenerator() = default;
  
  void generate(Chunk& chunk);
};

using WorldGenerator = std::shared_ptr<TerrainGenerator>;
  
} // namespace redi

#endif // REDI_WORLD_TERRAINGENERATOR_HPP