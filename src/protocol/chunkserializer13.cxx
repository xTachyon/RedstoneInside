#include "chunkserializer13.hpp"
#include "../world/block.hpp"
#include "../logger.hpp"

namespace redi
{

ChunkSerializer13::ChunkSerializer13(const Chunk& chunk, Vector2i pos, Dimension dimension)
      : mChunk(chunk), mPosition(pos), mDimension(dimension), packet(0x20) {}

ByteBuffer ChunkSerializer13::operator()()
{
  writeHeader(); // Header (position, etc.)
  writeChunkSections(packet); // Chunk sections
	writeBiomes(packet); // Biomes
  writeBlockEntities(packet); // A big 0
  
  packet.commit();
	return packet;
}

void ChunkSerializer13::writeHeader()
{
  std::size_t sectionsize =
        1 + // Bits per block
        1 + // Palette length
        2 + // VarInt data array length
        ChunkSectionDataSize * 8 + // Block data size
        LightDataSize; // Block light
  
  if (mDimension == Dimension::Overworld)
  {
    sectionsize += LightDataSize; // Skylight, if overworld
  }
  
  std::size_t size =
        sectionsize * 16 +
        BiomeDataSize;

  packet.writeInt(mPosition.x); // Chunk x
  packet.writeInt(mPosition.z); // Chunk z
  packet.writeBool(true); // Ground-Up Continuous
  packet.writeVarInt(0xFFFF); // Primary Bit Mask - Bitmask with bits set to 1 for every 16×16×16 chunk section whose data is included in Data.
  packet.writeVarInt(size); // Size of Data in bytes, plus size of Biomes in bytes if present
  
  Logger::debug((boost::format("Sending chunk %1%") % mPosition).str());
}

void ChunkSerializer13::writeChunkSections(PacketWriter& writer)
{
  for (std::uint8_t i = 0; i < 16; ++i)
  {
    writeChunkSection(writer, i);
  }
}

void ChunkSerializer13::writeChunkSection(PacketWriter& writer, std::uint8_t nth)
{
  writer.writeUByte(BitsPerBlock); // Bits Per Block
  writer.writeUByte(0); // Palette length
  writer.writeVarInt(ChunkSectionDataArraySize); // Data Array Length
  
  std::uint64_t temp = 0;
  std::uint64_t currentlyWrittenIndex = 0;
  
  std::size_t blockindex = 0;
  for (std::uint8_t x = 0; x < SectionX; ++x)
  {
    for (std::uint8_t y = 0; y < SectionY; ++y)
    {
      for (std::uint8_t z = 0; z < SectionZ; ++z)
      {
        std::uint64_t blockstate = generateBlockStateID(mChunk(y, x + SectionY * nth, z));

        std::size_t bitPosition = blockindex * BitsPerBlock;
        std::size_t firstIndex = bitPosition / 64;
        std::size_t secondIndex = ((blockindex + 1) * BitsPerBlock - 1) / 64;
        std::size_t bitOffset = bitPosition % 64;
        
        if (firstIndex != currentlyWrittenIndex)
        {
          packet.writeULong(temp);
          temp = 0;
          currentlyWrittenIndex = firstIndex;
        }
        
        temp |= blockstate << bitOffset;
        
        if (firstIndex != secondIndex)
        {
          packet.writeULong(temp);
          currentlyWrittenIndex = secondIndex;
          
          temp = (blockstate >> (64 - bitOffset));
        }
  
        ++blockindex;
      }
    }
  }
  
  packet.writeULong(temp);

  writeBlockLight(writer, nth); // Block Light
  if (mDimension == Dimension::Overworld)
  {
    writeSkyLight(writer, nth); // Sky Light
  }
}

void ChunkSerializer13::writeBlockLight(PacketWriter& writer, std::uint8_t)
{
  for (std::size_t i = 0; i < 2048; ++i)
  {
    writer.writeUByte(0xFF);
  }
}

void ChunkSerializer13::writeSkyLight(PacketWriter& writer, std::uint8_t)
{
  for (std::size_t i = 0; i < 2048; ++i)
  {
    writer.writeUByte(0xFF);
  }
}

void ChunkSerializer13::writeBiomes(PacketWriter& writer)
{
  const Chunk::ChunkColumns& c = mChunk.getChunkColumns();

  for (std::size_t x = 0; x < 16; ++x)
  {
    for (std::size_t z = 0; z < 16; ++z)
    {
      writer.writeUByte(static_cast<std::uint8_t>(static_cast<const ChunkColumn&>(c[x][z]).biome));
    }
  }
}

void ChunkSerializer13::writeBlockEntities(PacketWriter& writer)
{
  writer.writeVarInt(0);
}

std::uint64_t ChunkSerializer13::generateBlockStateID(Block b)
{
  constexpr std::uint64_t OverflowMask = (1 << BitsPerBlock) - 1;
  //constexpr std::uint64_t OverflowMask = 0b111111111;
  return ((static_cast<std::uint64_t>(b.type) & OverflowMask)
        << 4) | b.data;
}

} // namespace redi
