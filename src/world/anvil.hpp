#pragma once

#include <fstream>
#include <array>
#include <vector>
#include <boost/filesystem.hpp>
#include "../vectorn.hpp"
#include "../sizeliteraloperators.hpp"
#include "../bytebuffer.hpp"

namespace redi {
namespace world {

class Anvil {
public:
  struct ChunkInfo {
    std::uint32_t offset, time;
    std::uint8_t sectors;

    ChunkInfo(std::uint32_t offset = 0, std::uint32_t time = 0,
              std::uint8_t sectors = 0)
        : offset(offset), time(time), sectors(sectors) {}
  };

  enum class ChunkReadResult { OK, DoesntExists, Error };

  static constexpr std::size_t HeaderSize = 8_KB;
  static constexpr std::size_t SectorSize =
      4_KB; // uhh, sectors. Earth is on Sector 2814
  static constexpr std::int32_t ChunksPerRegion = 1 << 10;

  Anvil();
  Anvil(const std::string& path, const Vector2i& pos, bool create = true);
  Anvil(const boost::filesystem::path& path, const Vector2i& pos,
        bool create = true);
  ~Anvil();

  void open(const boost::filesystem::path& path, const Vector2i& pos,
            bool create = true);
  void close();
  void flush();
  ChunkInfo getChunkInfo(std::int32_t number) const;
  ChunkInfo getChunkInfo(const Vector2i& chunk) const;
  ChunkReadResult readChunk(const Vector2i& number, ByteBuffer& buffer);
  void writeChunk(const Vector2i& number, const ByteBuffer& uncompresseddata);

  static std::int32_t getChunkNumberInRegion(const Vector2i& other);
  static Vector2i getRegionCoordsFromChunkCoords(const Vector2i& chcoords);
  static size_t getSectorsNumber(std::size_t size);

private:
  std::fstream file;
  std::array<std::uint8_t, HeaderSize> header;
  std::vector<bool> freeSectors;
  Vector2i position;
  bool hasRegion;

  void clear();
  void createNewRegionAndOpen(const std::string& filepath);
  void openFile(const std::string& path);
  void createNewRegionIfOK(const boost::filesystem::path& filepath,
                           bool create);
  void readHeader();
  void processHeader();
  void writeChunkInfo(std::int32_t number, const ChunkInfo& info);
  void writeChunkInfo(const Vector2i& number, const ChunkInfo& chunk);
  ChunkReadResult readChunk(std::int32_t number, ByteBuffer& buffer);
  void writeChunk(std::int32_t number, const ByteBuffer& uncompresseddata);
};

} // namespace world
} // namespace redi