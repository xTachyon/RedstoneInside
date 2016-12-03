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
  {
    fs::path path(filepath);
    if (!fs::exists(path) || fs::file_size(path) == 0) createNewRegion(filepath);
    else if (fs::file_size(path) % 4 != 0 || fs::file_size(path) < HeaderSize)
    {
      Logger::warn(filepath + " has invalid size. Overwritting ...");
      createNewRegion(filepath);
    }
  }
  mFile.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
  if (!mFile) throw std::invalid_argument("can\'t open file " + filepath);
  readHeader();
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
  if (!file) throw std::invalid_argument("can\'t open file " + filepath);

  file.write(std::array<char, HeaderSize>().data(), HeaderSize);
}

void Region::readHeader()
{
  BinaryData buffer(HeaderSize, static_cast<std::uint8_t>(0));

  mFile.seekg(std::ios::beg);
  mFile.read(reinterpret_cast<char*>(buffer[0]), HeaderSize);

  for (std::size_t i = 0; i < ChunksPerRegion; ++i)
  {
    ChunkInfo& th = mChunks[i];

    th.sectors = buffer[i * 4 + 3];
    buffer[i * 4 + 3] = 0;
    std::memcpy(&th.offset, buffer.data() + i * 4, sizeof(std::int32_t));
    endian::big_to_native_inplace(th.offset);
    std::memcpy(&th.time, buffer.data() + SectorSize + i * 4, sizeof(std::int32_t));
  }
}

} // namespace redi