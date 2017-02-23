#ifndef REDI_WORLD_CHUNKMANAGER_HPP
#define REDI_WORLD_CHUNKMANAGER_HPP

#include <map>
#include "../vectorn.hpp"
#include "anvilregion.hpp"
#include "chunk.hpp"

namespace redi
{

class ChunkManager
{
public:
  
  ChunkManager(const std::string& regiondir, WorldGenerator generator);
  
  Block operator()(Vector3i pos);
  const Chunk& getChunk(Vector2i pos);
  
  void loadChunk(Vector2i pos);
  
private:
  
  std::map<Vector2i, world::AnvilRegion> mRegions;
  std::map<Vector2i, Chunk> mChunks;
  std::string mRegionDirectory;
  WorldGenerator mGenerator;
};
  
} // namespace redi

#endif // REDI_WORLD_CHUNKMANAGER_HPP