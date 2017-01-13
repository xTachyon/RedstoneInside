#include <boost/multi_array.hpp>
#include "chunk.hpp"
#include "nbt/nbt.hpp"
#include "sizeliteraloperators.hpp"

namespace redi
{

Chunk::Chunk() : mBlocks(boost::extents[16][256][16]) {}

Chunk& Chunk::operator=(const ByteBuffer& data)
{
  deserializeChunk(nbt::NBTDeserializer(data).get());
  return *this;
}

Chunk& Chunk::operator=(const nbt::RootTag& root)
{
  deserializeChunk(root);
  return *this;
}

void Chunk::deserializeChunk(const nbt::RootTag& root)
{
  const nbt::TagCompound& level = root["Level"]->get<nbt::NBTType::Compound>();
  const nbt::TagList& sections = level["Sections"]->get<nbt::NBTType::List>();
  
  deserializeSections(sections);
}

void Chunk::deserializeSections(const nbt::TagList& sections)
{
  for (auto& index : sections)
  {
    const nbt::TagCompound& idx = index->get<nbt::NBTType::Compound>();
    
    std::int8_t Y = idx["Y"]->get<nbt::NBTType::Byte>();
    if (!(0 <= Y && Y <= 15)) continue;
    
    const nbt::Int8Vector& blocks = idx["Blocks"]->get<nbt::NBTType::ByteArray>();
    if (blocks.size() != 4_KB) continue;
    
    const nbt::Int8Vector& data = idx["Data"]->get<nbt::NBTType::ByteArray>();
    if (data.size() != 2_KB) continue;
    
    const nbt::Int8Vector& blocklight = idx["BlockLight"]->get<nbt::NBTType::ByteArray>();
    if (blocklight.size() != 2_KB) continue;
    
    const nbt::Int8Vector& skylight = idx["SkyLight"]->get<nbt::NBTType::ByteArray>();
    if (skylight.size() != 2_KB) continue;
    
    boost::optional<const nbt::Int8Vector&> add;
    if (idx.keyExists("Add"))
    {
      add = idx["Add"]->get<nbt::NBTType::ByteArray>();
      if (add->size() != 2_KB) continue;
    }
    
    deserializeSection(Y, blocks, add, data, blocklight, skylight);
  }
}

void Chunk::deserializeSection(std::int8_t nthSection, const nbt::Int8Vector& blocks,
                               boost::optional<const nbt::Int8Vector&> add,
                               const nbt::Int8Vector& data, const nbt::Int8Vector& blocklight,
                               const nbt::Int8Vector& skylight)
{
  nthSection *= BlocksPerSection;
  
  for (std::size_t x = 0, k = 0; x < ChunkMaxX; ++x)
    for (std::size_t z = 0; z < ChunkMaxZ; ++z)
      for (std::size_t y = 0; y < BlocksPerSection; ++y)
        mBlocks[x][y + nthSection][z].type = static_cast<BlockType>(blocks[k++]);
  
  for (std::size_t x = 0, k = 0; x < ChunkMaxX; ++x)
    for (std::size_t z = 0; z < ChunkMaxZ; ++z)
      for (std::size_t y = 0; y < BlocksPerSection;)
      {
        mBlocks[x][(y++) + nthSection][z].data = data[k] >> 4;
        mBlocks[x][(y++) + nthSection][z].data = data[k++] & 0b00001111;
      }
  
  for (std::size_t x = 0, k = 0; x < ChunkMaxX; ++x)
    for (std::size_t z = 0; z < ChunkMaxZ; ++z)
      for (std::size_t y = 0; y < BlocksPerSection;)
      {
        mBlocks[x][(y++) + nthSection][z].blocklight = blocklight[k] >> 4;
        mBlocks[x][(y++) + nthSection][z].blocklight = blocklight[k++] & 0b00001111;
      }
  
  for (std::size_t x = 0, k = 0; x < ChunkMaxX; ++x)
    for (std::size_t z = 0; z < ChunkMaxZ; ++z)
      for (std::size_t y = 0; y < BlocksPerSection;)
      {
        mBlocks[x][(y++) + nthSection][z].skylight = skylight[k] >> 4;
        mBlocks[x][(y++) + nthSection][z].skylight = skylight[k++] & 0b00001111;
      }
  
  if (add)
  {
    static_cast<void>(add);
    // TODO: handle add
  }
}

void bestTerrainGenerator(Chunk& ch)
{
  for (std::int32_t x = 0; x < Chunk::ChunkMaxX; ++x)
    for (std::int32_t z = 0; z < Chunk::ChunkMaxZ; ++z)
      ch(x, 0, z) = BlockType::Bedrock;
  
  for (std::int32_t x = 0; x < Chunk::ChunkMaxX; ++x)
    for (std::int32_t z = 0; z < Chunk::ChunkMaxZ; ++z)
      ch(x, 1, z) = BlockType::Glowstone;
  
  for (std::int32_t x = 0; x < Chunk::ChunkMaxX; ++x)
    for (std::int32_t z = 0; z < Chunk::ChunkMaxZ; ++z)
      ch(x, 2, z) = BlockType::Leaves2;
}

} // namespace redi