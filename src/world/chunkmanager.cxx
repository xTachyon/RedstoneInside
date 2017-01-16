#include "chunkmanager.hpp"

namespace redi
{

ChunkManager::ChunkManager(const std::string& regiondir, WorldGenerator generator) : mRegionDirectory(regiondir), mGenerator(generator) {}

Block ChunkManager::operator()(Vector3i pos)
{
  
}

void ChunkManager::loadChunk(Vector2i pos)
{
  Vector2i chunkInRegion(pos.x / Region::ChunksPerRegion, pos.z / Region::ChunksPerRegion);
  if (mChunks.count(chunkInRegion) == 0)
  {
    // TODO: load chunk
    mGenerator->generate(mChunks[pos]);
  }
}
  
} // namespace redi