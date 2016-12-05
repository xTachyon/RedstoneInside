#ifndef REDI_REGION
#define REDI_REGION

#include <array>
#include <fstream>
#include <vector>
#include "sizeliteraloperators.hpp"
#include "position.hpp"
#include "binarydata.hpp"

namespace redi
{

struct ChunkInfo
{
  std::uint32_t offset, time;
  std::uint8_t sectors;

  ChunkInfo(std::int32_t offset = 0, std::int32_t  time = 0, std::uint8_t sectors = 0)
      : offset(offset), time(time), sectors(sectors) {}
};

class Region
{
public:

  static constexpr std::size_t ChunksPerRegion = 1 << 10;
  static constexpr std::size_t HeaderSize = 8_KB;
  static constexpr std::size_t SectorSize = 4_KB; // uhh, sectors. Earth is on Sector 2814
  static constexpr std::size_t ChunkHeaderSize = 5;

  Region() = default;
  Region(const std::string& filepath);
  ~Region();

  void clear();
  void close();
  void flush();
  void open(const std::string& filepath);
  BinaryData readChunk(const ChunkPosition& ch);
  void writeChunk(const ChunkPosition& ch, const BinaryData& data, bool updateDate = true);

  static void createNewRegion(const std::string& filepath);
  static std::int32_t getChunkNumberInRegion(const ChunkPosition& other);

private:
  
  std::fstream mFile;
  std::vector<bool> mFreeSectors;
  std::array<ChunkInfo, ChunksPerRegion> mChunks;
  bool hasRegion = false;

  void readHeader();
  void saveHeader();
};

} // namespace redi


#endif // REDI_REGION