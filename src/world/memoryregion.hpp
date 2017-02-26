#ifndef REDI_WORLD_MEMORYREGION_HPP
#define REDI_WORLD_MEMORYREGION_HPP

#include <map>
#include <boost/asio.hpp>
#include "anvilregion.hpp"
#include "chunk.hpp"

namespace redi
{
namespace world
{

class ChunkManager;
struct ChunkHolder;
class Player;

class MemoryRegion : public std::enable_shared_from_this<MemoryRegion>
{
public:
  
  MemoryRegion(ChunkManager& manager, boost::asio::io_service& io);
  
  /*
   * Only use those if you know what you are doing
   * If you misuse it, expect crashes or memory leaks
   * Or both
   */
  void increaseCount(const Vector2i& v);
  void decreaseCount(const Vector2i& v);
  void unloadChunk(const Vector2i& v);
  void loadChunk(const Vector2i& coordinates, Player* player);
  bool isLoaded(const Vector2i& coordinates) const { return chunks.count(coordinates) != 0; }
  
private:
  
  boost::asio::io_service& workIO;
  ChunkManager& manager;
  AnvilRegion region;
  Vector2i regionCoordinates;
  boost::asio::io_service::strand strand;
  std::map<Vector2i, std::pair<ChunkUniquePtr, std::int32_t>> chunks;
  std::map<Vector2i, std::vector<Player*>> playersWhoWants;
  std::int32_t count;
  
  /*
   * Only called from asio
   */
  void writeChunk(const Vector2i& l, const ChunkUniquePtr& chunk);
  void readChunk(const Vector2i& v);
};

using MemoryRegionSharedPtr = std::shared_ptr<MemoryRegion>;

struct ChunkHolder
{
  const MemoryRegion& ref;
};

} // namespace world
} // namespace redi

#endif // REDI_WORLD_MEMORYREGION_HPP