#ifndef REDI_WORLD
#define REDI_WORLD

#include <functional>
#include <string>
#include "chunk.hpp"
#include "region.hpp"
#include "position.hpp"

namespace redi
{

class World
{
public:
  
  using WorldGenerator = std::function<void(Chunk&)>;
  
  World(std::string&& worldname, std::string&& worlddir, WorldGenerator gen = bestTerrainGenerator)
        : mWorldName(std::move(worldname)), mDirectory(std::move(worlddir)), mGenerator(gen) {}
  World(std::string&& worldname, const std::string& worlddir, WorldGenerator gen = bestTerrainGenerator)
        : World(std::move(worldname), std::string(worlddir), gen) {}
  World(const std::string& worldname, std::string&& worlddir, WorldGenerator gen = bestTerrainGenerator)
        : World(std::string(worldname), std::move(worlddir), gen) {}
  World(const std::string& worldname, const std::string& worlddir, WorldGenerator gen = bestTerrainGenerator)
        : World(std::string(worldname), std::string(worlddir), gen) {}
  
private:

  std::string mWorldName;
  std::string mDirectory;
  WorldGenerator mGenerator;
  std::map<Vector2i, Region> mRegions;
  std::map<Vector2i, Chunk> mChunks;
};
  
} // namespace redi

#endif // REDI_WORLD
