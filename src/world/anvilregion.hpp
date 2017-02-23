#ifndef REDI_REGION
#define REDI_REGION

#include <array>
#include <fstream>
#include <vector>
#include "../sizeliteraloperators.hpp"
#include "../vectorn.hpp"
#include "../bytebuffer.hpp"

namespace redi
{
namespace world
{

struct ChunkInfo
{
  std::uint32_t offset, time;
  std::uint8_t sectors;

  ChunkInfo(std::int32_t offset = 0, std::int32_t  time = 0, std::uint8_t sectors = 0)
      : offset(offset), time(time), sectors(sectors) {}
};

class AnvilRegion
{
public:

  static constexpr std::int32_t ChunksPerRegion = 1 << 10;
  static constexpr std::size_t HeaderSize = 8_KB;
  static constexpr std::size_t SectorSize = 4_KB; // uhh, sectors. Earth is on Sector 2814
  static constexpr std::size_t ChunkHeaderSize = 5;

  AnvilRegion() = default;
  AnvilRegion(const std::string& filepath);
  ~AnvilRegion();

  void clear();
  void close();
  void flush();
  void open(const std::string& filepath);
  ByteBuffer readChunk(Vector2i ch);
  void writeChunk(Vector2i ch, const ByteBuffer& data, bool updateDate = true);

  static void createNewRegion(const std::string& filepath);
  static std::int32_t getChunkNumberInRegion(Vector2i other);

private:
  
  std::fstream mFile;
  std::vector<bool> mFreeSectors;
  std::array<ChunkInfo, ChunksPerRegion> mChunks;
  bool hasRegion = false;

  void readHeader();
  void saveHeader();
};

} // namespace world
} // namespace redi


#endif // REDI_REGION