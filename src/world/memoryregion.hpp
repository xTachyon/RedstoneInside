#ifndef REDI_WORLD_MEMORYREGION_HPP
#define REDI_WORLD_MEMORYREGION_HPP

#include <map>
#include <set>
#include <boost/asio.hpp>
#include "anvilregion.hpp"
#include "chunk.hpp"
#include "anvil.hpp"

namespace redi
{

class Player;
using PlayerSharedPtr = std::shared_ptr<Player>;

namespace world
{

class ChunkManager;
struct ChunkHolder;

class MemoryRegion : public std::enable_shared_from_this<MemoryRegion>
{
public:
  
  MemoryRegion(ChunkManager& manager, boost::asio::io_service& io, const Vector2i& coords);
  
  const Chunk& operator[](const Vector2i& coords) const { return *chunks.at(coords).first; }
  
  /*
   * Only use those if you know what you are doing
   * If you misuse it, expect crashes or memory leaks
   * Or both
   */
  void increaseCount(const Vector2i& v);
  void decreaseCount(const Vector2i& v);
  void unloadChunk(const Vector2i& v);
  void loadChunk(const Vector2i& coordinates, PlayerSharedPtr player = nullptr);
  bool isLoaded(const Vector2i& coordinates) const { return chunks.count(coordinates) != 0; }
  ChunkHolder addChunk(const Vector2i& coords, ChunkUniquePtr&& chunk);
  void addChunkAndNotifyPlayers(const Vector2i& coords, ChunkUniquePtr&& chunk);
  
private:
  
  using ChunksMapValueType = std::pair<ChunkUniquePtr, std::int32_t>;
  
  boost::asio::io_service& workIO;
  ChunkManager& manager;
  Anvil region;
  Vector2i regionCoordinates;
  boost::asio::io_service::strand strand;
  std::map<Vector2i, ChunksMapValueType> chunks;
  std::map<Vector2i, std::set<PlayerSharedPtr>> playersWhoWants;
  std::int32_t count;
  
  /*
   * Only called from asio
   */
  void writeChunk(const Vector2i& l, const Chunk& chunk);
  void readChunk(const Vector2i& v);
};

using MemoryRegionSharedPtr = std::shared_ptr<MemoryRegion>;

struct ChunkHolder
{
  ChunkHolder() : ref(nullptr) {}
  
  ChunkHolder(MemoryRegion* ref, const Vector2i& coords)
        : ref(ref), coords(coords)
  {
    inc();
  }
  
  ChunkHolder(MemoryRegion& ref, const Vector2i& coords)
        : ChunkHolder(&ref, coords) {}
  
  ChunkHolder(const ChunkHolder& chunk) : ChunkHolder(chunk.ref, chunk.coords) {}
  
  ChunkHolder(ChunkHolder&& chunk)
  {
    ref = chunk.ref;
    coords = chunk.coords;
    
    chunk.ref = nullptr;
  }
  
  ChunkHolder& operator=(const ChunkHolder& chunk)
  {
    dec();
    ref = chunk.ref;
    coords = chunk.coords;
    inc();
    
    return *this;
  }
  
  ChunkHolder& operator=(ChunkHolder&& chunk)
  {
    dec();
    ref = chunk.ref;
    coords = chunk.coords;
    
    return *this;
  }
  
  const Chunk& operator*() const
  {
    if (ref)
    {
      return (*ref)[coords];
    }
    throw std::runtime_error("No chunk in this ChunkHolder");
  }
  
  ~ChunkHolder()
  {
    dec();
  }
  
  const Vector2i& getCoords() const { return coords; }
  
private:
  
  MemoryRegion* ref;
  Vector2i coords;
  
  void inc()
  {
    if (ref)
    {
      ref->increaseCount(coords);
    }
  }
  
  void dec()
  {
    if (ref)
    {
      ref->decreaseCount(coords);
    }
  }
};

} // namespace world
} // namespace redi

#endif // REDI_WORLD_MEMORYREGION_HPP