#pragma once

#include "../world/chunk.hpp"
#include "../serverconfig.hpp"
#include "packetwriter.hpp"

namespace redi {

class ChunkSerializer13 {
public:
  ChunkSerializer13(const Chunk& chunk, Vector2i pos,
                    Dimension dimension = Dimension::Overworld);

  ByteBuffer operator()();

private:
  static constexpr std::size_t ChunkSectionsNumber = 16;
  static constexpr std::size_t BitsPerBlock = 13;
  static constexpr std::size_t BlocksPerSection = 16 * 16 * 16;
  static constexpr std::size_t ChunkSectionDataSize =
      BlocksPerSection * BitsPerBlock / 8 / 8;
  static constexpr std::size_t BiomeDataSize = 256;
  static constexpr std::size_t LightDataSize = BlocksPerSection / 2;
  static constexpr std::uint8_t SectionX = 16;
  static constexpr std::uint8_t SectionY = 16;
  static constexpr std::uint8_t SectionZ = 16;
  static constexpr std::size_t ChunkSectionDataArraySize =
      (BlocksPerSection * BitsPerBlock) / 8 /
      8; // Convert from bit count to long count

  const Chunk& mChunk;
  Vector2i mPosition;
  Dimension mDimension;
  ByteBuffer mBuffer;
  PacketWriter packet;

  void writeHeader();
  void writeChunkSections();
  void writeChunkSection(std::uint8_t nth);
  void writeBlockLight(std::uint8_t nth);
  void writeSkyLight(std::uint8_t nth);
  void writeBiomes();
  void writeBlockEntities();

  static uint64_t generateBlockStateID(Block b);
};

} // namespace redi