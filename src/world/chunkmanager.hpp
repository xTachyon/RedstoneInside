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
  ChunkHolder operator()(const Vector2i& coords) const;
  const Chunk& getChunk(Vector2i pos);
  
  void loadChunk(const Vector2i& pos, PlayerSharedPtr player = nullptr);
  void unloadRegion(const Vector2i& pos);
  bool isChunkLoaded(const Vector2i& coords) const;
  
  Server& getServer() { return server; }
  const std::string& getRegionDirectory() const { return mRegionDirectory; }
  
  WorldGenerator& getWorldGenerator() { return mGenerator; }
  
  private:
  
  Server& server;
  boost::asio::io_service& workIO;
  std::map<Vector2i, world::MemoryRegionSharedPtr> regions;
  std::map<Vector2i, world::AnvilRegion> mRegions;
  std::map<Vector2i, Chunk> mChunks;
  std::string mRegionDirectory;
  WorldGenerator mGenerator;
};

} // namespace world
} // namespace redi

#endif // REDI_WORLD_CHUNKMANAGER_HPP