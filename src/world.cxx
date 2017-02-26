#include <boost/filesystem.hpp>
#include "player.hpp"
#include "world.hpp"

namespace fs = boost::filesystem;

namespace redi
{

World::World(Server& server, const std::string& worldname, const std::string& worlddir, WorldGenerator ptr, Dimension dim)
  : server(server), mWorldName(worldname), mDirectory(worlddir), mGenerator(ptr),
    mChunkManager(server, mDirectory, redi::WorldGenerator()),
    mDimension(dim), worldTime(8000)
{
  fs::create_directories(mDirectory);
}

void World::addPlayer(Player& player)
{
  mPlayers.push_back(&player);
}

void World::deletePlayer(Player& player)
{
  mPlayers.remove(&player);
}
  
} // namespace redi