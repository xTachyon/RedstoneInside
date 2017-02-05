#include "chunkdata.hpp"
#include "../../../packetwriternocopy.hpp"
#include "../../../chunkserializer13.hpp"

namespace redi
{
namespace packets
{

ChunkData::ChunkData(const Chunk& chunk, Vector2i position) : chunk(chunk), position(position) {}

void ChunkData::write(ByteBuffer& buffer)
{
  buffer = ChunkSerializer13(chunk, position)();
}
  
} // namespace packets
} // namespace redi