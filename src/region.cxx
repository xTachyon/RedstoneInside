#include <boost/filesystem.hpp>
#include <boost/endian/conversion.hpp>
#include "region.hpp"
#include "binarydata.hpp"
#include "logger.hpp"

namespace fs = boost::filesystem;
namespace endian = boost::endian;

namespace redi
{

Region::Region(const std::string& filepath)
{
  open(filepath);
}

void Region::open(const std::string& filepath)
{
  clear();
  fs::path path(filepath);
  if (!fs::exists(path)) createNewRegion(filepath);
  std::uintmax_t size = fs::file_size(path);
  if (size % 4 != 0 || size < HeaderSize) createNewRegion(filepath);
  mFile.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
  if (!mFile) throw std::invalid_argument("can\'t open file " + filepath);
  mFreeSectors.resize(size / SectorSize);
  std::fill(mFreeSectors.begin(), mFreeSectors.end(), true);
  readHeader();
  hasRegion = true;
}

void Region::clear()
{
  mFile.close();
  mFreeSectors.clear();
  std::fill(mChunks.begin(), mChunks.end(), ChunkInfo());
  hasRegion = false;
}

void Region::createNewRegion(const std::string& filepath)
{
  std::ofstream file(filepath, std::ios::binary);
  if (!file) throw std::invalid_argument("can't open file " + filepath);

  file.write(std::array<char, HeaderSize>().data(), HeaderSize);
}

void Region::readHeader()
{
  BinaryData buffer(HeaderSize, static_cast<std::uint8_t>(0));

  mFile.seekg(std::ios::beg);
  mFile.read(reinterpret_cast<char*>(&buffer[0]), HeaderSize);
  mFreeSectors[0] = mFreeSectors[1] = false;

  for (std::size_t i = 0; i < ChunksPerRegion; ++i)
  {
    ChunkInfo& th = mChunks[i];

    th.sectors = buffer[i * 4 + 3];
    buffer[i * 4 + 3] = 0;
    std::memcpy(reinterpret_cast<std::uint8_t*>(&th.offset) + 1, buffer.data() + i * 4, sizeof(std::int32_t) - 1);
    endian::big_to_native_inplace(th.offset);
    std::memcpy(&th.time, buffer.data() + SectorSize + i * 4, sizeof(std::int32_t));
    endian::big_to_native_inplace(th.time);

    if (th.offset + th.sectors > mFreeSectors.size())
    {
      Logger::warn(std::to_string(i) + " is out of range");
      th.offset = th.time = 0;
      th.sectors = 0;
      continue;
    }
    for (std::size_t j = th.offset, max = th.offset + th.sectors; j < max; ++j)
      mFreeSectors[j] = false;
  }
}

} // namespace redi