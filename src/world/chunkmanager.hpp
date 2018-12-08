#pragma once

#include <map>
#include "../vectorn.hpp"
#include "anvilregion.hpp"
#include "chunk.hpp"
#include "memoryregion.hpp"

namespace redi {

class Server;

namespace world {

class ChunkManager {
public:
  ChunkManager(Server& server, const std::string& regiondir,
               WorldGenerator generator);

  ChunkHolder operator()(const Vector2i& coords) const;

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
  std::string mRegionDirectory;
  WorldGenerator mGenerator;
};

} // namespace world
} // namespace redi