#ifndef REDI_WORLD_CHUNKSERIALIZER13_HPP
#define REDI_WORLD_CHUNKSERIALIZER13_HPP

#include "../world/chunk.hpp"
#include "../serverconfig.hpp"
#include "packetwriter.hpp"

namespace redi
{

class ChunkSerializer13
{
  public:
  
  ChunkSerializer13(const Chunk& chunk, Vector2i pos, Dimension dimension = Dimension::Overworld);
  
  ByteBuffer operator()();
  
  private:
  
  static constexpr std::size_t ChunkSectionsNumber = 16;
  static constexpr std::size_t BitsPerBlock = 13;
  static constexpr std::size_t BlocksPerSection = 16 * 16 * 16;
  static constexpr std::size_t ChunkSectionDataSize = BlocksPerSection * BitsPerBlock / 8 / 8;
  static constexpr std::size_t BiomeDataSize = 256;
  
  const Chunk& mChunk;
  Vector2i mPosition;
  Dimension mDimension;
  
  void writeHeader(PacketWriter& writer);
  void writeChunkSections(PacketWriter& writer);
  void writeChunkSection(PacketWriter& writer, std::uint8_t nth);
  void writeBlockLight(PacketWriter& writer, std::uint8_t nth);
  void writeSkyLight(PacketWriter& writer, std::uint8_t nth);
  void writeBiomes(PacketWriter& writer);
  void writeBlockEntities(PacketWriter& writer);
  
  static uint64_t generateBlockStateID(Block b);
};
  
} // namespace redi

#endif // REDI_CHUNKSERIALIZER13_HPP
