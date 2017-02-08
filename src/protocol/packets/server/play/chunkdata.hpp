#ifndef REDI_CHUNKDATA_HPP
#define REDI_CHUNKDATA_HPP

#include "../../packet.hpp"
#include "../../../../world/chunk.hpp"

namespace redi
{
namespace packets
{

struct ChunkData : public Packet
{
  static constexpr std::int32_t SendID = 0x20;
  
  const Chunk& chunk;
  Vector2i position;
  
  ChunkData(const Chunk& chunk, Vector2i position);
  
  void write(ByteBuffer& buffer) override;
};

} // namespace packets
} // namespace redi

#endif // REDI_CHUNKDATA_HPP