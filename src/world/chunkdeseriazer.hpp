#ifndef REDI_WORLD_CHUNKTONBT_HPP
#define REDI_WORLD_CHUNKTONBT_HPP

#include "../bytebuffer.hpp"
#include "../nbt/nbt.hpp"
#include "chunk.hpp"

namespace redi
{
namespace world
{

struct ChunkDeserializer
{
  const Chunk& chunk;
  nbt::RootTag& root;

  ChunkDeserializer(const Chunk& chunk, nbt::RootTag& root);

  void operator()();
};

} // namespace world
} // namespace redi

#endif // REDI_CHUNKTONBT_HPP