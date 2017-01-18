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
  
  static constexpr std::size_t PaletteBits = 13;
  static constexpr std::size_t BlocksPerSection = 16 * 16 * 16;
  static constexpr std::size_t SectionsSizeWithoutSkyLight =
        1 + // Bits per block
        1 + // Palette length
        2 +
        BlocksPerSection * PaletteBits / 8 / 8 * 8 +
        128;
  
  
  const Chunk& mChunk;
  Vector2i mPosition;
  Dimension mDimension;
  
  void writeChunkSections(PacketWriter& writer);
  void writeChunkSection(PacketWriter& writer, std::uint8_t nth);
  
  static std::int32_t generateBlockStateID(Block b);
  static std::size_t getChunkSize(Dimension d = Dimension::Overworld);
};
  
} // namespace redi

#endif // REDI_CHUNKSERIALIZER13_HPP
