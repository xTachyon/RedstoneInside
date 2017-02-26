#include <boost/filesystem.hpp>
#include "chunkmanager.hpp"
#include "../server.hpp"

namespace fs = boost::filesystem;

namespace redi
{
namespace world
{

ChunkManager::ChunkManager(Server& server, const std::string& regiondir, WorldGenerator generator)
      : server(server), workIO(server.getWorkIO()),
        mRegionDirectory(regiondir + "/region"), mGenerator(generator)
{
  fs::create_directories(mRegionDirectory);
}

Block ChunkManager::operator()(Vector3i pos)
{
  Vector2i c(pos.x / 16, pos.z / 16);
  if (mChunks.count(c) == 0)
  {
    loadChunk(c, redi::PlayerSharedPtr());
  }
  
  return mChunks[c](pos.x % 16, pos.y, pos.z % 16);
}

void ChunkManager::loadChunk(const Vector2i& coords, PlayerSharedPtr player)
{
//  Vector2i chunkInRegion(pos.x / world::AnvilRegion::ChunksPerRegion, pos.z / world::AnvilRegion::ChunksPerRegion);
//  if (mRegions.count(chunkInRegion) == 0)
//  {
//    // TODO: load chunk
//    mGenerator->generate(mChunks[pos]);
//  }
  
  Vector2i regionCoords(world::AnvilRegion::getRegionCoordsFromChunkCoords(coords));
  if (regions.count(regionCoords) == 0)
  {
    regions[regionCoords] = std::make_shared<world::MemoryRegion>(*this, workIO, regionCoords);
  }
  
  regions[regionCoords]->loadChunk(coords, player);
}

const Chunk& ChunkManager::getChunk(Vector2i c)
{
  if (mChunks.count(c) == 0)
  {
    loadChunk(c, redi::PlayerSharedPtr());
  }
  
  return mChunks[c];
}

void ChunkManager::unloadRegion(const Vector2i& pos)
{
  regions.erase(pos);
}

bool ChunkManager::isChunkLoaded(const Vector2i& coords) const
{
  Vector2i regionCoords(world::AnvilRegion::getRegionCoordsFromChunkCoords(coords));
  if (regions.count(regionCoords) == 0)
  {
    return false;
  }
  
  return regions.at(regionCoords)->isLoaded(coords);
}

ChunkHolder ChunkManager::operator()(const Vector2i& coords) const
{
  if (!isChunkLoaded(coords))
  {
    throw std::runtime_error("Chunk is not loaded");
  }
  
  Vector2i regionCoords = world::AnvilRegion::getRegionCoordsFromChunkCoords(coords);
  auto& ch = *regions.at(regionCoords);
  ChunkHolder c(ch, coords);
  return c;
//  return (*regions.at(regionCoords))[coords];
}
  
} // namespace world
} // namespace redi