#include "chunkdeseriazer.hpp"

namespace redi
{
namespace world
{

ChunkDeserializer::ChunkDeserializer(const ByteBuffer& buffer, const Chunk& chunk)
      : buffer(buffer), chunk(chunk) {}

  
} // namespace world
} // namespace redi