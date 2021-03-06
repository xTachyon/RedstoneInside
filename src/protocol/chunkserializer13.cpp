#include "chunkserializer13.hpp"
#include "../world/block.hpp"
#include "../logger.hpp"

namespace redi {

ChunkSerializer13::ChunkSerializer13(const Chunk& chunk, Vector2i pos,
                                     Dimension dimension)
    : mChunk(chunk), mPosition(pos), mDimension(dimension),
      packet(mBuffer, 0x20) {}

ByteBuffer ChunkSerializer13::operator()() {
  writeHeader();        // Header (position, etc.)
  writeChunkSections(); // Chunk sections
  writeBiomes();        // Biomes
  writeBlockEntities(); // A big 0

  return mBuffer;
}

void ChunkSerializer13::writeHeader() {
  std::size_t sectionsize = 1 + // Bits per block
                            1 + // Palette length
                            2 + // VarInt data array length
                            ChunkSectionDataSize * 8 + // Block data size
                            LightDataSize;             // Block light

  if (mDimension == Dimension::Overworld) {
    sectionsize += LightDataSize; // Skylight, if overworld
  }

  std::size_t size = sectionsize * 16 + BiomeDataSize;

  packet.writeInt(mPosition.x); // Chunk x
  packet.writeInt(mPosition.z); // Chunk z
  packet.writeBool(true);       // Ground-Up Continuous
  packet.writeVarInt(0xFFFF);   // Primary Bit Mask - Bitmask with bits set to 1
                              // for every 16×16×16 chunk section whose data is
                              // included in Data.
  packet.writeVarInt(
      size); // Size of Data in bytes, plus size of Biomes in bytes if present

  //  Logger::debug((boost::format("Sending chunk %1%") % mPosition).str());
}

void ChunkSerializer13::writeChunkSections() {
  for (std::uint8_t i = 0; i < 16; ++i) {
    writeChunkSection(i);
  }
}

void ChunkSerializer13::writeChunkSection(std::uint8_t nth) {
  packet.writeUByte(BitsPerBlock);               // Bits Per Block
  packet.writeUByte(0);                          // Palette length
  packet.writeVarInt(ChunkSectionDataArraySize); // Data Array Length

  std::uint64_t temp = 0;
  std::uint64_t currentlyWrittenIndex = 0;

  std::size_t blockindex = 0;
  for (std::uint8_t y = 0; y < SectionY; ++y) {
    for (std::uint8_t z = 0; z < SectionZ; ++z) {
      for (std::uint8_t x = 0; x < SectionX; ++x) {
        std::uint64_t blockstate =
            generateBlockStateID(mChunk(x, y + SectionY * nth, z));

        std::size_t bitPosition = blockindex * BitsPerBlock;
        std::size_t firstIndex = bitPosition / 64;
        std::size_t secondIndex = ((blockindex + 1) * BitsPerBlock - 1) / 64;
        std::size_t bitOffset = bitPosition % 64;

        if (firstIndex != currentlyWrittenIndex) {
          packet.writeULong(temp);
          temp = 0;
          currentlyWrittenIndex = firstIndex;
        }

        temp |= blockstate << bitOffset;

        if (firstIndex != secondIndex) {
          packet.writeULong(temp);
          currentlyWrittenIndex = secondIndex;

          temp = (blockstate >> (64 - bitOffset));
        }

        ++blockindex;
      }
    }
  }

  packet.writeULong(temp);

  writeBlockLight(nth); // Block Light
  if (mDimension == Dimension::Overworld) {
    writeSkyLight(nth); // Sky Light
  }
}

void ChunkSerializer13::writeBlockLight(std::uint8_t) {
  for (std::size_t i = 0; i < 2048; ++i) {
    packet.writeUByte(0xFF);
  }
}

void ChunkSerializer13::writeSkyLight(std::uint8_t) {
  for (std::size_t i = 0; i < 2048; ++i) {
    packet.writeUByte(0xFF);
  }
}

void ChunkSerializer13::writeBiomes() {
  const Chunk::ChunkColumns& c = mChunk.getChunkColumns();

  for (std::size_t x = 0; x < 16; ++x) {
    for (std::size_t z = 0; z < 16; ++z) {
      packet.writeUByte(static_cast<std::uint8_t>(
          static_cast<const ChunkColumn&>(c[x][z]).biome));
    }
  }
}

void ChunkSerializer13::writeBlockEntities() { packet.writeVarInt(0); }

std::uint64_t ChunkSerializer13::generateBlockStateID(Block b) {
  constexpr std::uint64_t OverflowMask = (1 << BitsPerBlock) - 1;
  return ((static_cast<std::uint64_t>(b.type) & OverflowMask) << 4) | b.data;
}

} // namespace redi