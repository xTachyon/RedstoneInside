#include "memoryregion.hpp"
#include "chunkmanager.hpp"
#include "../logger.hpp"
#include "../nbt/roottag.hpp"
#include "../nbt/serializer.hpp"
#include "chunkdeseriazer.hpp"
#include "chunkserializer.hpp"
#include "../events/events.hpp"
#include "../server.hpp"

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
  try
  {
    nbt::RootTag root;
    
    ChunkSerializer(root, *chunk)();
    
    ByteBuffer buffer;
    nbt::Serializer(buffer).write(root);
    region.flush();
  
    region.writeChunk(l, buffer);
  }
  catch (std::exception& e)
  {
    Logger::error(e.what());
  }
}

void MemoryRegion::readChunk(const Vector2i& v)
{
  try
  {
    ByteBuffer buffer = region.readChunk(v);
    
    ChunkUniquePtr ptr = std::make_unique<Chunk>();
    nbt::RootTag root;
    
    nbt::Deserializer(buffer).read(root);
    ChunkDeserializer(*ptr, root)();
    region.flush();
  
    manager.getServer()
          .addEvent(std::make_unique<events::EventChunkLoaded>(std::move(ptr), *this, v));
  }
  catch (std::exception& e)
  {
    Logger::error(e.what());
  }
}
  
} // namespace world
} // namespace redi