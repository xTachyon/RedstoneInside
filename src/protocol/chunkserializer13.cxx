#include "chunkserializer13.hpp"
#include "../world/block.hpp"

namespace redi
{

ChunkSerializer13::ChunkSerializer13(const Chunk& chunk, Vector2i pos, Dimension dimension)
      : mChunk(chunk), mPosition(pos), mDimension(dimension) {}

ByteBuffer ChunkSerializer13::operator()()
{
  PacketWriter packet(0x20); // packet id

//  writeHeader(writer);
//  writeChunkSections(writer);
//  writeBiomes(writer);
//  writeBlockEntities(writer);
//
//  writer.commit();
//  return writer;

	packet.writeInt(mPosition.x);
	packet.writeInt(mPosition.z);
	packet.writeBool(true);        // "Ground-up continuous", or rather, "biome data present" flag
	packet.writeVarInt(0xFFFF);  // We're aways sending the full chunk with no additional data, so the bitmap is 0xffff
	// Write the chunk size:
    const size_t ChunkSectionDataArraySize = (BlocksPerSection * BitsPerBlock) / 8 / 8;  // Convert from bit count to long count
	std::size_t ChunkSectionSize = (
		1 +
		1 +
		2 +
		ChunkSectionDataArraySize * 8 +
		BlocksPerSection / 2
	);

	if (mDimension == Dimension::Overworld)
	{
		// Sky light is only sent in the overworld.
		ChunkSectionSize += BlocksPerSection / 2;
	}

	const size_t BiomeDataSize = 16 * 16;
	size_t ChunkSize = (
		ChunkSectionSize * 16 +
		BiomeDataSize
	);
	packet.writeVarInt(ChunkSize);

	// Write each chunk section...
	for (size_t index = 0; index < 16; index++)
	{
		packet.writeByte(BitsPerBlock);
		packet.writeVarInt(0);  // Palette length is 0
		packet.writeVarInt(ChunkSectionDataArraySize);

		for (std::size_t i = 0; i < ChunkSectionDataArraySize; ++i)
		{
		    //srand(time(nullptr));
			packet.writeLong(1);
		}

		// Light - stored as a nibble, so we need half sizes
		// As far as I know, there isn't a method to only write a range of the array
		writeBlockLight(packet, index);
		//for (size_t Index = 0; Index < ChunkSectionBlocks / 2; Index++)
		//{
		//	packet.writeUByte(0xFF);
		//}
		if (mDimension == Dimension::Overworld)
		{
			// Skylight is only sent in the overworld; the nether and end do not use it
			//for (size_t Index = 0; Index < ChunkSectionBlocks / 2; Index++)
			//{
			//	packet.writeUByte(0xFF);
			//}
			writeBlockLight(packet, index);
		}
	}

	// Write the biome data
//	Packet.WriteBuf(m_BiomeData, BiomeDataSize);
	writeBiomes(packet);
	// Identify 1.9.4's tile entity list as empty
	packet.writeUByte(0);

	packet.commit();
	return packet;
}

void ChunkSerializer13::writeHeader(PacketWriter& writer)
{
  std::size_t sectionsize =
        1 +
        1 +
        2 +
        ChunkSectionDataSize * 8 +
        BlocksPerSection / 2;

  if (mDimension == Dimension::Overworld)
  {
    sectionsize += BlocksPerSection / 2;
  }

  std::size_t chunksize = sectionsize * 16 + BiomeDataSize;

  writer.writeInt(mPosition.x); // chunk x
  writer.writeInt(mPosition.z); // chunk z
  writer.writeBool(true); // Ground-Up Continuous
  writer.writeVarInt(0xFFFF); // Primary Bit Mask - Bitmask with bits set to 1 for every 16×16×16 chunk section whose data is included in Data.
  writer.writeVarInt(chunksize); // Size of Data in bytes, plus size of Biomes in bytes if present
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
  writer.writeVarInt(ChunkSectionDataSize); // Data Array Length

//  std::uint64_t value = 0;
//  std::size_t offset = 0;
//  std::size_t shift = 0;

//  for (std::uint16_t x = 0; x < Chunk::ChunkMaxX; ++x)
//  {
//    for (std::uint16_t z = 0; z < Chunk::ChunkMaxZ; ++z)
//    {
//      for (std::uint16_t y = 0; y < 16; ++y)
//      {
//        std::uint64_t blockstate = generateBlockStateID(mChunk(x, 16 * nth, z));
//
//        if (offset + 13 <= 64)
//        {
//          shift = 64 - offset - 13;
//          value |= blockstate << shift;
//          offset += 13;
//        }
//        else
//        {
//          shift = 13 - (64 - offset);
//          value |= blockstate >> (13 - shift);
//          writer.writeVarLong(value);
//          value = 0;
//          offset = shift;
//          shift = 64 - (13 - shift);
//          value = blockstate << shift;
//        }
//      }
//    }
//  }

  for (std::size_t i = 0; i < ChunkSectionDataSize; ++i)
  {
    writer.writeVarLong(0);
  }

//  writer.writeVarLong(value);
  writeBlockLight(writer, nth); // Block Light
  if (mDimension == Dimension::Overworld) writeSkyLight(writer, nth); // Sky Light
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
