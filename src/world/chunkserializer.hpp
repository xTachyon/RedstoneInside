#ifndef REDI_WORLD_CHUNKSERIALIZER_HPP
#define REDI_WORLD_CHUNKSERIALIZER_HPP

#include "chunk.hpp"
#include "../nbt/nbt.hpp"
#include "../datatypes.hpp"

namespace redi {
namespace world {

struct ChunkSerializer {
  const Chunk& chunk;
  nbt::tag_compound& root;

  ChunkSerializer(nbt::root_tag& root, const Chunk& chunk);

  void operator()();

private:
  static nbt::tag_compound& resolve(nbt::root_tag& root);

  void writeMisc();

  void writeSections();
  void writeSection(nbt::tag_list& list, std::uint8_t y);
  bool writeBlocks(byte* bytes, std::int16_t yy);
  //  void writeAdd(const std::vector<std::int8_t>& buffer, std::int16_t yy);
  void writeData(byte* bytes, std::int16_t yy);
  void writeBlockLight(byte* bytes, std::int16_t yy);
  void writeSkyLight(byte* bytes, std::int16_t yy);
};

} // namespace world
} // namespace redi

#endif // REDI_WORLD_CHUNKSERIALIZER_HPP