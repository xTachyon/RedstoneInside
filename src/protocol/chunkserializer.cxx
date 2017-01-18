#include "chunkserializer.hpp"
#include "packetwriter.hpp"
#include "../logger.hpp"

namespace redi
{

ChunkSerializer::ChunkSerializer(const Chunk& chunk, Vector2i pos, Dimension dimension)
      : mChunk(chunk), mPosition(pos), mDimension(dimension) {}

ByteBuffer ChunkSerializer::operator()()
{
  PacketWriter writer(0x20);
  
  writer.writeInt(mPosition.x); // chunk x
  writer.writeInt(mPosition.z); // chunk z
  writer.writeBool(true); // Ground-Up Continuous
  writer.writeVarInt(0xFFFF); //
  
  writeChunkSections(writer);
  
  for (std::size_t i = 0; i < 256; ++i)
  {
    writer.writeByte(0);
  }
  
  writer.writeVarInt(0);
  writer.commit();
  
  return writer.data;
}

void ChunkSerializer::writeChunkSections(PacketWriter& writer)
{
  PacketWriter w(0x00);
  w.data.clear();
  
  for (std::uint8_t i = 0; i < 16; ++i)
  {
    writeChunkSection(w, i);
  }
  
  writer.writeVarInt(256 + w.data.size());
  writer.data += w.data;
}

void ChunkSerializer::writeChunkSection(PacketWriter& writer, std::uint8_t nth)
{
  writer.writeUByte(8);
  
  std::vector<std::int32_t> palette;
  std::array<std::int64_t, 512> data;
  
  generatePaletteAndDataArray(palette, data, static_cast<std::uint8_t>(nth * 16));
  
  writer.writeVarInt(palette.size());
  for (std::int32_t i : palette)
  {
    writer.writeVarInt(i);
  }
  
  writer.writeVarInt(data.size());
  for (std::int64_t i : data)
  {
    writer.writeVarLong(i);
  }
  
  for (std::size_t i = 0; i < (4096 / 2); ++i)
  {
    writer.writeByte(0xFF);
  }
  
  if (mDimension == Dimension::Overworld)
  {
    for (std::size_t i = 0; i < (4096 / 2); ++i)
    {
      writer.writeByte(0xFF);
    }
  } // TODO: implement skylight, blocklight
}

void ChunkSerializer::generatePaletteAndDataArray(std::vector<std::int32_t>& palette, std::array<std::int64_t, 512>& data,
                                                  std::uint8_t plusy)
{
  std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(data.data());
  std::size_t offset = 0;
  
  for (std::uint16_t x = 0; x < Chunk::ChunkMaxX; ++x)
  {
    for (std::uint16_t z = 0; z < Chunk::ChunkMaxZ; ++z)
    {
      for (std::uint16_t y = 0; y < 16; ++y)
      {
        std::int32_t blockstate = generateBlockStateID(mChunk(x, y + plusy, z));
        
        std::size_t found_at = 0;
        bool found = false;
        
        for (std::size_t i = 0; i < palette.size() && !found; ++i)
        {
          if (palette[i] == blockstate)
          {
            found_at = i;
            found = true;
          }
        }
        
        if (found) ptr[offset++] = static_cast<std::uint8_t>(found_at);
        else
        {
          palette.push_back(blockstate);
          ptr[offset++] = static_cast<std::uint8_t>(palette.size() - 1);
        }
      }
    }
  }
}

std::int32_t ChunkSerializer::generateBlockStateID(Block b)
{
  return (static_cast<std::int32_t>(b.type) << 4) | b.data;
}
  
} // namespace redi