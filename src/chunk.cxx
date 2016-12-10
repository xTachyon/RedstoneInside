#include <boost/multi_array.hpp>
#include "chunk.hpp"
#include "nbt/nbt.hpp"

namespace redi
{

Chunk::Chunk() : mBlocks(boost::extents[16][256][16]) {}

Chunk& Chunk::operator=(const BinaryData& data)
{
  deserializeChunk(nbt::NBTDeserializer(data));
  return *this;
}

Chunk& Chunk::operator=(const nbt::RootTag& root)
{
  deserializeChunk(root);
  return *this;
}

void Chunk::deserializeChunk(const nbt::RootTag& root)
{
  const nbt::TagCompound& level = root["Level"].get().get<nbt::NBTType::Compound>();
  const nbt::TagList& sections = level["Sections"].get().get<nbt::NBTType::List>();
  
  //deserializeBiomes(level["Biomes"].get().get<nbt::NBTType::IntArray>());
}

void Chunk::deserializeSections(const nbt::TagList& sections)
{
  for (const auto& index : sections)
  {
    const nbt::TagCompound& th = index->get<nbt::NBTType::Compound>();
    //deserializeSection();
  }
}

void Chunk::deserializeSection(std::int8_t nthSection, const nbt::TagByteArray& blocks,
                               boost::optional<const nbt::TagByteArray&> add,
                               const nbt::TagByteArray& data, const nbt::TagByteArray& blocklight,
                               const nbt::TagByteArray& skylight)
{
  
}
  
  
} // namespace redi
