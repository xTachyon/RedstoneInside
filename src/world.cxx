#include "player.hpp"
#include "world.hpp"

namespace redi
{

World::World(const std::string& worldname, const std::string& worlddir, WorldGenerator ptr, Dimension dim)
  : mWorldName(worldname), mDirectory(worlddir), mGenerator(ptr),
    mChunkManager(mDirectory + "/region", redi::WorldGenerator()),
    mDimension(dim)
{}

void World::addPlayer(Player* player)
{
  mPlayers.push_back(player);
}

void World::deletePlayer(Player* player)
{
  mPlayers.remove(player);
}
  
} // namespace redi