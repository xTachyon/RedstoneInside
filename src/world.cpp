#include <boost/filesystem.hpp>
#include "player.hpp"
#include "world.hpp"

namespace fs = boost::filesystem;

namespace redi {

World::World(Server& server, const std::string& worldname,
             const std::string& worlddir, WorldGenerator ptr, Dimension dim)
    : HasServer(server), mWorldName(worldname), mDirectory(worlddir),
      mGenerator(ptr), mChunkManager(server, mDirectory, ptr), dimension(dim),
      worldTime(8000) {
  static_cast<void>(dimension);
  // TODO: remove this after using dimension
}

void World::addPlayer(Player& player) {
  mPlayers.push_back(&player);
}

void World::deletePlayer(Player& player) {
  mPlayers.remove(&player);
}

} // namespace redi