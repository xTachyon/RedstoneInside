#ifndef REDI_WORLD
#define REDI_WORLD

#include <functional>
#include <list>
#include <string>
#include "world/chunk.hpp"
#include "world/region.hpp"
#include "vectorn.hpp"
#include "world/chunkmanager.hpp"
#include "serverconfig.hpp"

namespace redi
{

class Player;

class World
{
public:

  World(const std::string& worldname, const std::string& worlddir, WorldGenerator ptr, Dimension dim = Dimension::Overworld);
  ChunkManager& getChunkManager() { return mChunkManager; }
  void addPlayer(Player* player);
  void deletePlayer(Player* player);
  
private:

  std::string mWorldName;
  std::string mDirectory;
  WorldGenerator mGenerator;
  ChunkManager mChunkManager;
  Dimension mDimension;
  std::list<Player*> mPlayers;
};
  
} // namespace redi

#endif // REDI_WORLD
