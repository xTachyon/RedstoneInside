#include "chunkserializer13.hpp"

namespace redi
{

ChunkSerializer13::ChunkSerializer13(const Chunk& chunk, Vector2i pos, Dimension dimension)
      : mChunk(chunk), mPosition(pos), mDimension(dimension) {}

ByteBuffer ChunkSerializer13::operator()()
{
  PacketWriter writer(0x20); // packet id
  
  writer.writeInt(mPosition.x); // chunk x
  writer.writeInt(mPosition.z); // chunk z
  writer.writeBool(true); // Ground-Up Continuous
  writer.writeVarInt(0xFFFF); // Primary Bit Mask - Bitmask with bits set to 1 for every 16×16×16 chunk section whose data is included in Data.
  writer.writeVarInt(getChunkSize(mDimension)); // Size of Data in bytes, plus size of Biomes in bytes if present
  
  
  return writer;
  
}

std::size_t ChunkSerializer13::getChunkSize(Dimension d)
{
  std::size_t chunksize = SectionsSizeWithoutSkyLight;
  if (d == Dimension::Overworld) chunksize += 128;
  
  return chunksize * 16 + 256;
}
  
} // namespace redi