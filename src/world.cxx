#include "world.hpp"

namespace redi
{

World::World(const std::string& worldname, const std::string& worlddir, WorldGenerator ptr, Dimension dim)
  : mWorldName(worldname), mDirectory(worlddir), mGenerator(ptr),
    mChunkManager(mDirectory + "/region", redi::WorldGenerator()),
    mDimension(dim)
{}


} // namespace redi