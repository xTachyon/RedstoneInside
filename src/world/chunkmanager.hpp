#ifndef REDI_WORLD_CHUNKMANAGER_HPP
#define REDI_WORLD_CHUNKMANAGER_HPP

#include <map>
#include "../vectorn.hpp"
#include "anvilregion.hpp"
#include "chunk.hpp"
#include "memoryregion.hpp"

namespace redi
{

class Server;

namespace world
{

class ChunkManager
{
public:
  
  ChunkManager(Server& server, const std::string& regiondir, WorldGenerator generator);
  
  Block operator()(Vector3i pos);
  const Chunk& getChunk(Vector2i pos);
  
  void loadChunk(Vector2i pos);
  void unloadRegion(const Vector2i& pos);
  
  Server& getServer() { return server; }
  
private:
  
  Server& server;
  std::map<Vector2i, world::MemoryRegionSharedPtr> regions;
  std::map<Vector2i, world::AnvilRegion> mRegions;
  std::map<Vector2i, Chunk> mChunks;
  std::string mRegionDirectory;
  WorldGenerator mGenerator;
};

} // namespace world
} // namespace redi

#endif // REDI_WORLD_CHUNKMANAGER_HPP