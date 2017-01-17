#ifndef REDI_CHUNKSERIALIZER_HPP
#define REDI_CHUNKSERIALIZER_HPP

#include "../world/chunk.hpp"
#include "../serverconfig.hpp"
#include "packetwriter.hpp"

namespace redi
{

class ChunkSerializer
{
public:
  
  ChunkSerializer(const Chunk& chunk, Vector2i pos, Dimension dimension = Dimension::Overworld);
  
  ByteBuffer operator()();
  
private:
  
  const Chunk& mChunk;
  Vector2i mPosition;
  Dimension mDimension;
  
  void writeChunkSections(PacketWriter& writer);
  void writeChunkSection(PacketWriter& writer, std::uint8_t nth);
  void generatePaletteAndDataArray(std::vector<std::int32_t>& palette, std::array<std::int64_t, 512>& data,
                                     std::uint8_t plusy);
  
  static std::int32_t generateBlockStateID(Block b);
};

} // namespace redi

#endif //REDI_CHUNKSERIALIZER_HPP
