#ifndef REDI_WORLD
#define REDI_WORLD

#include <functional>
#include <string>
#include "world/chunk.hpp"
#include "world/region.hpp"
#include "vectorn.hpp"
#include "world/chunkmanager.hpp"
#include "serverconfig.hpp"

namespace redi
{

class World
{
public:

  World(const std::string& worldname, const std::string& worlddir, WorldGenerator ptr, Dimension dim = Dimension::Overworld);
  ChunkManager& getChunkManager() { return mChunkManager; }
  
private:

  std::string mWorldName;
  std::string mDirectory;
  WorldGenerator mGenerator;
  ChunkManager mChunkManager;
  Dimension mDimension;
};
  
} // namespace redi

#endif // REDI_WORLD
