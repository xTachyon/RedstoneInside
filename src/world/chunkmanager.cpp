#include <boost/filesystem.hpp>
#include "chunkmanager.hpp"
#include "../server.hpp"

namespace fs = boost::filesystem;

namespace redi {
namespace world {

ChunkManager::ChunkManager(Server& server, const std::string& regiondir,
                           WorldGenerator generator)
    : server(server), workIO(server.getWorkIO()),
      mRegionDirectory(regiondir + "/region"), mGenerator(generator) {
  fs::create_directories(mRegionDirectory);
}

void ChunkManager::loadChunk(const Vector2i& coords, PlayerSharedPtr player) {
  Vector2i regionCoords(
      world::AnvilRegion::getRegionCoordsFromChunkCoords(coords));
  if (regions.count(regionCoords) == 0) {
    regions[regionCoords] =
        std::make_shared<world::MemoryRegion>(*this, workIO, regionCoords);
  }

  regions[regionCoords]->loadChunk(coords, player);
}

void ChunkManager::unloadRegion(const Vector2i& pos) { regions.erase(pos); }

bool ChunkManager::isChunkLoaded(const Vector2i& coords) const {
  Vector2i regionCoords(
      world::AnvilRegion::getRegionCoordsFromChunkCoords(coords));
  if (regions.count(regionCoords) == 0) {
    return false;
  }

  return regions.at(regionCoords)->isLoaded(coords);
}

ChunkHolder ChunkManager::operator()(const Vector2i& coords) const {
  if (!isChunkLoaded(coords)) {
    throw std::runtime_error("Chunk is not loaded");
  }

  Vector2i regionCoords =
      world::AnvilRegion::getRegionCoordsFromChunkCoords(coords);
  //  auto& ch = *regions.at(regionCoords);
  //  ChunkHolder c(ch, coords);
  //  return c;
  //  return (*regions.at(regionCoords))[coords];
  return ChunkHolder(*regions.at(regionCoords), coords);
}

} // namespace world
} // namespace redi