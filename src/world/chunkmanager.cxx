#include "chunkmanager.hpp"

namespace redi
{

ChunkManager::ChunkManager(const std::string& regiondir, WorldGenerator generator) : mRegionDirectory(regiondir), mGenerator(generator) {}

Block ChunkManager::operator()(Vector3i pos)
{
  Vector2i c(pos.x / 16, pos.z / 16);
  if (mChunks.count(c) == 0)
  {
    loadChunk(c);
  }
  
  return mChunks[c](pos.x % 16, pos.y, pos.z % 16);
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

const Chunk& ChunkManager::getChunk(Vector2i c)
{
  if (mChunks.count(c) == 0)
  {
    loadChunk(c);
  }
  
  return mChunks[c];
}
  
} // namespace redi