#include "../sizeliteraloperators.hpp"
#include "chunkserializer.hpp"

namespace redi {
namespace world {

ChunkSerializer::ChunkSerializer(nbt::RootTag& root, const Chunk& chunk)
    : chunk(chunk), root(resolve(root)) {}

void ChunkSerializer::operator()() {
  writeMisc();
  writeSections();
}

void ChunkSerializer::writeMisc() {
  root["xPos"] = chunk.position.x;
  root["zPos"] = chunk.position.z;
  root["InhabitedTime"] = chunk.inhabitedTime;
}

nbt::TagCompound& ChunkSerializer::resolve(nbt::RootTag& root) {
  root["Level"] = nbt::create(nbt::Type::Compound);
  return root["Level"].getCompound();
}

void ChunkSerializer::writeSections() {
  root["Sections"] = nbt::create(nbt::Type::List);
  nbt::TagList& list = root["Sections"].getList();

  for (std::uint8_t i = 0; i < 16; ++i) {
    writeSection(list, i);
  }
}

void ChunkSerializer::writeSection(nbt::TagList& list, std::uint8_t y) {
  std::vector<sbyte> buffer;
  nbt::TagCompound comp;
  std::int16_t yy = y * y;

  {
    buffer.resize(4_KB);
    if (!writeBlocks(reinterpret_cast<byte*>(buffer.data()), yy)) {
      return;
    }

    comp["Blocks"] = nbt::TagByteArray(std::move(buffer));
  }

  {
      // add
  }

  {
    buffer.resize(2_KB);
    writeData(reinterpret_cast<byte*>(buffer.data()), yy);

    comp["Data"] = nbt::TagByteArray(std::move(buffer));
  }

  {
    buffer.resize(2_KB);
    writeBlockLight(reinterpret_cast<byte*>(buffer.data()), yy);

    comp["BlockLight"] = nbt::TagByteArray(std::move(buffer));
  }

  {
    buffer.resize(2_KB);
    writeSkyLight(reinterpret_cast<byte*>(buffer.data()), yy);

    comp["SkyLight"] = nbt::TagByteArray(std::move(buffer));
  }

  { comp["Y"] = nbt::TagByte(y); }

  list.push_back(std::move(comp));
}

bool ChunkSerializer::writeBlocks(byte* bytes, std::int16_t yy) {
  bool has = false;

  for (std::uint8_t y = 0; y < 16; ++y) {
    for (std::uint8_t z = 0; z < 16; ++z) {
      for (std::uint8_t x = 0; x < 16; ++x) {
        *bytes = static_cast<byte>(chunk(x, y + yy, z).type);
        if (*bytes != 0) {
          has = true;
        }
        ++bytes;
      }
    }
  }

  return has;
}

void ChunkSerializer::writeData(byte* bytes, std::int16_t yy) {
  for (std::uint8_t y = 0; y < 16; ++y) {
    for (std::uint8_t z = 0; z < 16; ++z) {
      for (std::uint8_t x = 0; x < 16; x += 2) {
        *bytes = util::createNibble(chunk(x, y + yy, z).data,
                                    chunk(x + 1, y + yy, z).data);
        ++bytes;
      }
    }
  }
}

void ChunkSerializer::writeBlockLight(byte* bytes, int16_t yy) {
  for (std::uint8_t y = 0; y < 16; ++y) {
    for (std::uint8_t z = 0; z < 16; ++z) {
      for (std::uint8_t x = 0; x < 16; x += 2) {
        *bytes = util::createNibble(chunk(x, y + yy, z).blocklight,
                                    chunk(x + 1, y + yy, z).blocklight);
      }
    }
  }
}

void ChunkSerializer::writeSkyLight(byte* bytes, std::int16_t yy) {
  for (std::uint8_t y = 0; y < 16; ++y) {
    for (std::uint8_t z = 0; z < 16; ++z) {
      for (std::uint8_t x = 0; x < 16; x += 2) {
        *bytes = util::createNibble(chunk(x, y + yy, z).skylight,
                                    chunk(x + 1, y + yy, z).skylight);
      }
    }
  }
}

} // namespace world
} // namespace redi
