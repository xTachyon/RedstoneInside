#ifndef REDI_EVENTS_CHUNKLOADED_HPP
#define REDI_EVENTS_CHUNKLOADED_HPP

#include "event.hpp"
#include "../world/memoryregion.hpp"
#include "../player.hpp"

namespace redi
{
namespace events
{

struct EventChunkLoaded : public Event
{
  ChunkUniquePtr chunk;
  world::MemoryRegion& region;
  Vector2i coordinates;
  
  EventChunkLoaded(ChunkUniquePtr&& chunk, world::MemoryRegion& region, const Vector2i& coords)
        : Event(EventType::ChunkLoaded), chunk(std::move(chunk)), region(region), coordinates(coords) {}
  
};

} // namespace events
} // namespace redi

#endif // REDI_EVENTS_CHUNKLOADED_HPP