#include "chunkdeseriazer.hpp"

namespace redi {
namespace world {

ChunkDeserializer::ChunkDeserializer(Chunk& chunk, const nbt::RootTag& root)
    : chunk(chunk), root(root.at("Level").getCompound()) {}

void ChunkDeserializer::operator()() {
  readMisc();
  readSections();
}

void ChunkDeserializer::readMisc() {
  chunk.position.x = root.at("xPos").getInt();
  chunk.position.z = root.at("zPos").getInt();
  chunk.inhabitedTime = root.at("InhabitedTime").getLong();
}

void ChunkDeserializer::readSections() {
  for (const nbt::Value& index : root.at("Sections").getList().get()) {
    readSection(index.getCompound());
  }
}

void ChunkDeserializer::readSection(const nbt::TagCompound& section) {
  std::uint8_t y = static_cast<std::uint8_t>(section.at("Y").getByte());
  y *= y;

  readBlocks(section.at("Blocks").getByteArray(), y);
  if (section.exists("Add")) {
    readAdd(section.at("Add").getByteArray(), y);
  }
  readData(section.at("Data").getByteArray(), y);
  readBlockLight(section.at("BlockLight").getByteArray(), y);
  readSkyLight(section.at("SkyLight").getByteArray(), y);
}

void ChunkDeserializer::readBlocks(const std::vector<std::int8_t>& buffer,
                                   std::int16_t yy) {
//  const std::uint8_t* ptr =
//      reinterpret_cast<const std::uint8_t*>(buffer.data());
  std::size_t k = 0;

  for (std::uint8_t y = 0; y < 16; ++y) {
    for (std::uint8_t z = 0; z < 16; ++z) {
      for (std::uint8_t x = 0; x < 16; ++x) {
        chunk(x, y + yy, z).type = static_cast<BlockType>(
            reinterpret_cast<const std::uint8_t&>(buffer[k++]));
      }
    }
  }
}

void ChunkDeserializer::readAdd(const std::vector<std::int8_t>&, std::int16_t) {
  // not yet
}

void ChunkDeserializer::readData(const std::vector<std::int8_t>& buffer,
                                 std::int16_t yy) {
  const std::uint8_t* ptr =
      reinterpret_cast<const std::uint8_t*>(buffer.data());
  std::size_t k = 0;

  for (std::uint8_t y = 0; y < 16; ++y) {
    for (std::uint8_t z = 0; z < 16; ++z) {
      for (std::uint8_t x = 0; x < 16;) {
        auto nibble = util::splitNibble(ptr[k++]);

        chunk(x, y + yy, z).data = nibble.first;
        ++x;

        chunk(x, y + yy, z).data = nibble.second;
        ++x;
      }
    }
  }
}

void ChunkDeserializer::readBlockLight(const std::vector<std::int8_t>& buffer,
                                       std::int16_t yy) {
  const std::uint8_t* ptr =
      reinterpret_cast<const std::uint8_t*>(buffer.data());
  std::size_t k = 0;

  for (std::uint8_t y = 0; y < 16; ++y) {
    for (std::uint8_t z = 0; z < 16; ++z) {
      for (std::uint8_t x = 0; x < 16;) {
        auto nibble = util::splitNibble(ptr[k++]);

        chunk(x, y + yy, z).blocklight = nibble.first;
        ++x;

        chunk(x, y + yy, z).blocklight = nibble.second;
        ++x;
      }
    }
  }
}

void ChunkDeserializer::readSkyLight(const std::vector<std::int8_t>& buffer,
                                     std::int16_t yy) {
  const std::uint8_t* ptr =
      reinterpret_cast<const std::uint8_t*>(buffer.data());
  std::size_t k = 0;

  for (std::uint8_t y = 0; y < 16; ++y) {
    for (std::uint8_t z = 0; z < 16; ++z) {
      for (std::uint8_t x = 0; x < 16;) {
        auto nibble = util::splitNibble(ptr[k++]);

        chunk(x, y + yy, z).skylight = nibble.first;
        ++x;

        chunk(x, y + yy, z).skylight = nibble.second;
        ++x;
      }
    }
  }
}

} // namespace world
} // namespace redi