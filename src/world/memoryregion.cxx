#include "memoryregion.hpp"
#include "chunkmanager.hpp"
#include "../logger.hpp"
#include "../nbt/roottag.hpp"
#include "../nbt/serializer.hpp"
#include "chunkdeseriazer.hpp"
#include "chunkserializer.hpp"
#include "../events/events.hpp"
#include "../server.hpp"
#include "../player.hpp"

namespace redi {
namespace world {

MemoryRegion::MemoryRegion(ChunkManager& manager, boost::asio::io_service& io,
                           const Vector2i& coords)
    : workIO(io), manager(manager), regionCoordinates(coords), strand(io),
      count(0) {
  region.open((boost::format("%1%/r.%2%.%3%.mca") %
               manager.getRegionDirectory() % regionCoordinates.x %
               regionCoordinates.z)
                  .str(),
              coords);
}

void MemoryRegion::increaseCount(const Vector2i& v) {
  //  assert(chunks.count(v));
  ++chunks[v].second;
  ++count;
}

void MemoryRegion::decreaseCount(const Vector2i& v) {
  assert(chunks.count(v));
  auto& c = chunks[v].second;
  --c;
  if (c == 0) {
    unloadChunk(v);
  }
  --count;
  if (count == 0) {
    //    manager.unloadRegion(regionCoordinates);
    // TODO: what happens when no chunk is used anymore ?
  }
}

void MemoryRegion::loadChunk(const Vector2i& coordinates,
                             PlayerSharedPtr player) {
  if (player) {
    playersWhoWants[coordinates].insert(player);
  }

  workIO.post(strand.wrap([ me = shared_from_this(), coordinates ] {
    me->readChunk(coordinates);
  }));
}

void MemoryRegion::unloadChunk(const Vector2i& v) {
  assert(chunks.count(v));

  ChunkUniquePtr ptr = std::move(chunks[v].first);
  chunks.erase(v);

  workIO.post(strand.wrap([
    me = shared_from_this(), v, chunk = std::shared_ptr<Chunk>(std::move(ptr))
  ] { me->writeChunk(v, std::move(*chunk)); }));
}

void MemoryRegion::writeChunk(const Vector2i& l, const Chunk& chunk) {
  try {
    nbt::RootTag root;

    ChunkSerializer(root, chunk)();

    ByteBuffer buffer;
    nbt::Serializer(buffer).write(root);

    region.writeChunk(l, buffer);
  } catch (std::exception& e) {
    Logger::error(e.what());
  }

  region.flush();
}

void MemoryRegion::readChunk(const Vector2i& v) {
  ChunkUniquePtr chunk = std::make_unique<Chunk>();
  chunk->position = v;

  try {
    ByteBuffer buffer;

    Anvil::ChunkReadResult result = region.readChunk(v, buffer);

//    switch (result) {
//    case Anvil::ChunkReadResult::OK: {
//      nbt::RootTag root;
//
//      nbt::Deserializer(buffer).read(root);
//      ChunkDeserializer (*chunk, root)();
//    } break;
//
//    case Anvil::ChunkReadResult::Error: {
//      Logger::error("Error reading chunk " + v.toString());
//    }
//
//    case Anvil::ChunkReadResult::DoesntExists: {
//      manager.getWorldGenerator()->generate(*chunk);
//    } break;
//    }

    manager.getWorldGenerator()->generate(*chunk);

  } catch (std::exception& e) {
    Logger::error(e.what());
    manager.getWorldGenerator()->generate(*chunk);
  }

  manager.getServer().addEvent(
      std::make_unique<events::EventChunkLoaded>(std::move(chunk), *this, v));

  region.flush();
}

ChunkHolder MemoryRegion::addChunk(const Vector2i& coords,
                                   ChunkUniquePtr&& chunk) {
  ChunksMapValueType pair(std::move(chunk), 0);
  chunks[coords] = std::move(pair);

  ChunkHolder c(*this, coords);
  return c;
}

void MemoryRegion::addChunkAndNotifyPlayers(const Vector2i& coords,
                                            ChunkUniquePtr&& chunk) {
  ChunkHolder holder(addChunk(coords, std::move(chunk)));

  for (PlayerSharedPtr player : playersWhoWants[coords]) {
    player->onChunkLoaded(holder);
  }

  playersWhoWants.erase(coords);
}

} // namespace world
} // namespace redi