#include <boost/filesystem.hpp>
#include "region.hpp"
#include "binarydata.hpp"

namespace fs = boost::filesystem;

namespace redi
{

Region::Region() : hasRegion(false) {}

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
  BinaryData header(HeaderSize, static_cast<std::uint8_t>(0));

  mFile.seekg(std::ios::beg);
  mFile.read(reinterpret_cast<char*>(header[0]), HeaderSize);
}

} // namespace redi