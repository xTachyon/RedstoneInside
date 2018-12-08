#pragma once

#include "../bytebuffer.hpp"
#include "../nbt/nbt.hpp"
#include "chunk.hpp"

namespace redi {
namespace world {

struct ChunkDeserializer {
  Chunk& chunk;
  const nbt::tag_compound& root;

  ChunkDeserializer(Chunk& chunk, const nbt::root_tag& root);

  void operator()();

private:
  void readMisc();

  void readSections();
  void readSection(const nbt::tag_compound& section);
  void readBlocks(const std::vector<std::int8_t>& buffer, std::int16_t yy);
  void readAdd(const std::vector<std::int8_t>& buffer, std::int16_t yy);
  void readData(const std::vector<std::int8_t>& buffer, std::int16_t yy);
  void readBlockLight(const std::vector<std::int8_t>& buffer, std::int16_t yy);
  void readSkyLight(const std::vector<std::int8_t>& buffer, std::int16_t yy);
};

} // namespace world
} // namespace redi