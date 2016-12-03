#ifndef REDI_REGION
#define REDI_REGION

#include <array>
#include <fstream>
#include <vector>
#include "sizeliteraloperators.hpp"

namespace redi
{

struct ChunkInfo
{
  std::int32_t offset, time;
  std::uint8_t sectors;

  ChunkInfo(std::int32_t offset = 0, std::int32_t  time = 0, std::uint8_t sectors = 0)
      : offset(offset), time(time), sectors(sectors) {}
};

class Region
{
public:

  static constexpr std::size_t ChunksPerRegion = 1 << 10;
  static constexpr std::size_t HeaderSize = 8_KB;
  static constexpr std::size_t SectorSize = 4_KB;

  Region();
  Region(const std::string& filepath);

  void open(const std::string& filepath);
  void clear();

  static void createNewRegion(const std::string& filepath);

private:
  
  std::fstream mFile;
  std::vector<bool> mFreeSectors;
  std::array<ChunkInfo, ChunksPerRegion> mChunks;
  bool hasRegion;

  void readHeader();
};

} // namespace redi


#endif // REDI_REGION