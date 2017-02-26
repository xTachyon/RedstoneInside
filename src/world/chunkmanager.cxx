#include <boost/filesystem.hpp>
#include "chunkmanager.hpp"

namespace fs = boost::filesystem;

namespace redi
{
namespace world
{

ChunkManager::ChunkManager(const std::string& regiondir, WorldGenerator generator)
      : mRegionDirectory(regiondir + "/region"), mGenerator(generator)
{
  fs::create_directories(mRegionDirectory);
}

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
  Vector2i chunkInRegion(pos.x / world::AnvilRegion::ChunksPerRegion, pos.z / world::AnvilRegion::ChunksPerRegion);
  if (mRegions.count(chunkInRegion) == 0)
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

void ChunkManager::unloadRegion(const Vector2i& pos)
{
  regions.erase(pos);
}

} // namespace world
} // namespace redi