#ifndef REDI_WORLD_CHUNKSERIALIZER_HPP
#define REDI_WORLD_CHUNKSERIALIZER_HPP

#include "chunk.hpp"
#include "../nbt/nbt.hpp"

namespace redi
{
namespace world
{

struct ChunkSerializer
{
  const Chunk& chunk;
  nbt::RootTag& root;
  
  ChunkSerializer(nbt::RootTag& root, const Chunk& chunk);
  
  void operator()();
};
  
} // namespace world
} // namespace redi

#endif // REDI_WORLD_CHUNKSERIALIZER_HPP