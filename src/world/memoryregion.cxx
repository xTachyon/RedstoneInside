#include "memoryregion.hpp"
#include "chunkmanager.hpp"

namespace redi
{
namespace world
{

MemoryRegion::MemoryRegion(ChunkManager& manager, boost::asio::io_service& io)
      : workIO(io), manager(manager), strand(io), count(0) {}

void MemoryRegion::increaseCount(const Vector2i& v)
{
  assert(chunks.count(v));
  ++chunks[v].second;
  ++count;
}

void MemoryRegion::decreaseCount(const Vector2i& v)
{
  assert(chunks.count(v));
  auto& c = chunks[v].second;
  --c;
  if (c == 0)
  {
    unloadChunk(v);
  }
  --count;
  if (count == 0)
  {
    manager.unloadRegion(regionCoordinates);
  }
}

void MemoryRegion::loadChunk(const Vector2i& coordinates, Player* player)
{
  workIO.post(strand.wrap([me = shared_from_this(), coordinates]
  {
    me->readChunk(coordinates);
  }));
}

void MemoryRegion::unloadChunk(const Vector2i& v)
{
  assert(chunks.count(v));
  
  ChunkUniquePtr ptr = std::move(chunks[v].first);
  chunks.erase(v);
  
  workIO.post(strand.wrap([me = shared_from_this(), v, &ptr]
  {
    me->writeChunk(v, std::move(ptr));
  }));
}

void MemoryRegion::writeChunk(const Vector2i& l, const ChunkUniquePtr& chunk)
{
  
}

void MemoryRegion::readChunk(const Vector2i& v)
{
  ChunkUniquePtr ptr = std::make_unique<Chunk>();
}
  
} // namespace world
} // namespace redi