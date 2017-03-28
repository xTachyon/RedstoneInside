#include <boost/filesystem.hpp>
#include <boost/endian/conversion.hpp>
#include "anvilregion.hpp"
#include "../bytebuffer.hpp"
#include "../logger.hpp"
#include "../compressor.hpp"
#include "../util/util.hpp"

namespace fs = boost::filesystem;
namespace endian = boost::endian;

namespace redi {
namespace world {

AnvilRegion::AnvilRegion(const std::string& filepath) { open(filepath); }

AnvilRegion::~AnvilRegion() { close(); }

void AnvilRegion::open(const std::string& filepath) {
  clear();
  fs::path path(filepath);
  if (!fs::exists(path))
    createNewRegion(filepath);
  std::uintmax_t size = fs::file_size(path);
  if (size % 4 != 0 || size < HeaderSize)
    createNewRegion(filepath);
  mFile.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
  if (!mFile)
    throw std::invalid_argument("can't open file " + filepath);
  mFreeSectors.resize(static_cast<std::size_t>(size / SectorSize));
  std::fill(mFreeSectors.begin(), mFreeSectors.end(), true);
  readHeader();
  hasRegion = true;
}

ByteBuffer AnvilRegion::readChunk(Vector2i ch) {
  ByteBuffer result(ChunkHeaderSize, '\0');
  std::int32_t chunknumber = getChunkNumberInRegion(ch);
  ChunkInfo& th = mChunks[chunknumber];

  if (th.offset == 0)
    return ByteBuffer();

  mFile.seekg(th.offset * SectorSize);
  mFile.read(reinterpret_cast<char*>(&result[0]), ChunkHeaderSize);

  std::int32_t size;
  std::memcpy(&size, &result[0], sizeof(std::int32_t));
  endian::big_to_native_inplace(size);

  if (size < 5) {
    return {};
  }

  result.resize(--size);
  std::uint8_t compressionFormat = result[4];
  mFile.read(reinterpret_cast<char*>(&result[0]), size);

  switch (compressionFormat) {
  case 1:
    result = compressor::decompressGzip(result);
    break;

  case 2:
    result = compressor::decompressZlib(result);
    break;

  default:
    result.clear();
    break;
  }

  return result;
}

void AnvilRegion::writeChunk(Vector2i ch, const ByteBuffer& data,
                             bool updateDate) {
  ByteBuffer result(ChunkHeaderSize, '\0');
  std::int32_t chunknumber = getChunkNumberInRegion(ch);
  ChunkInfo& th = mChunks[chunknumber];
  ByteBuffer dataToBeWritten(compressor::compressZlib(data));
  std::size_t newSectorsCount =
      ((dataToBeWritten.size() + 5) % SectorSize == 0)
          ? (dataToBeWritten.size() + 5) / SectorSize
          : (dataToBeWritten.size() + 5) / SectorSize + 1;

  if (th.sectors == newSectorsCount) {
    /*
    * If the actual sector count is equal
    * with the old one, just overwrite.
    */
    mFile.seekp(th.offset * SectorSize);
  } else {
    for (std::size_t i = th.offset; i < th.offset + th.sectors; ++i)
      mFreeSectors[i] = true;
    // Mark old sectors as free

    std::int32_t start = -1;
    std::size_t len;
    // If start is -1, no good sectors were found
    for (std::size_t i = 0; i < mFreeSectors.size(); ++i)
      if (mFreeSectors[i]) {
        start = static_cast<std::int32_t>(i);
        for (; i < mFreeSectors.size() && mFreeSectors[i]; ++i)
          ;
        len = i - start;
        if (len == newSectorsCount)
          break;
        // Perfect
      }

    if (start == -1) {
      /*
      * If no good sectors were found
      * add at the end of the file.
      */
      mFile.seekp(0, mFile.end);
      if (data.size() % 4096 != 0)
        dataToBeWritten.resize(dataToBeWritten.size() +
                               (4096 - dataToBeWritten.size() % 4096));
      // Pad with zeros
      th.offset = static_cast<std::int32_t>(mFreeSectors.size());
      th.sectors = static_cast<std::uint8_t>(newSectorsCount);
      for (std::size_t i = 0; i < th.sectors; ++i)
        mFreeSectors.push_back(false);
    } else {
      // Good sectors were found.
      mFile.seekp(start * SectorSize);
      th.offset = static_cast<std::int32_t>(start);
      th.sectors = static_cast<std::uint8_t>(newSectorsCount);
    }
  }

  char buffer[5];
  buffer[4] = 2;
  chunknumber = endian::native_to_big(static_cast<std::int32_t>(
      dataToBeWritten.size() + 1)); // recycling variables
  // TODO: is it +1 or -1 here ?
  std::memcpy(buffer, &chunknumber, sizeof(std::int32_t));
  mFile.write(buffer, 5);
  mFile.write(reinterpret_cast<const char*>(&dataToBeWritten[0]),
              dataToBeWritten.size());
  if (updateDate)
    th.time = util::getUnixTimestamp();
}

void AnvilRegion::clear() {
  mFile.close();
  mFreeSectors.clear();
  std::fill(mChunks.begin(), mChunks.end(), ChunkInfo());
  hasRegion = false;
}

void AnvilRegion::close() {
  saveHeader();
  clear();
}

void AnvilRegion::flush() {
  saveHeader();
  mFile << std::flush;
}

void AnvilRegion::createNewRegion(const std::string& filepath) {
  std::ofstream file(filepath, std::ios::binary);
  if (!file)
    throw std::invalid_argument("can't open file " + filepath);

  file.write(std::array<char, HeaderSize>().data(), HeaderSize);
}

std::int32_t AnvilRegion::getChunkNumberInRegion(Vector2i other) {
  return (other.x & 31) + (other.z & 31) * 32;
}

void AnvilRegion::readHeader() {
  ByteBuffer buffer(HeaderSize, static_cast<std::uint8_t>(0));

  mFile.seekg(std::ios::beg);
  mFile.read(reinterpret_cast<char*>(&buffer[0]), HeaderSize);
  mFreeSectors[0] = mFreeSectors[1] = false;

  for (std::size_t i = 0; i < ChunksPerRegion; ++i) {
    ChunkInfo& th = mChunks[i];

    th.sectors = buffer[i * 4 + 3];
    buffer[i * 4 + 3] = 0;
    std::memcpy(reinterpret_cast<std::uint8_t*>(&th.offset) + 1,
                buffer.data() + i * 4, sizeof(std::int32_t) - 1);
    endian::big_to_native_inplace(th.offset);
    std::memcpy(&th.time, buffer.data() + SectorSize + i * 4,
                sizeof(std::int32_t));
    endian::big_to_native_inplace(th.time);

    if (th.offset + th.sectors > mFreeSectors.size()) {
      Logger::warn(std::to_string(i) + " is out of range");
      th.offset = th.time = 0;
      th.sectors = 0;
      continue;
    }
    for (std::size_t j = th.offset, max = th.offset + th.sectors; j < max; ++j)
      mFreeSectors[j] = false;
  }
}

void AnvilRegion::saveHeader() {
  ByteBuffer buf(HeaderSize, static_cast<std::uint8_t>(0));
  std::uint8_t* buffer = &buf[0];

  for (std::size_t i = 0; i < ChunksPerRegion; ++i) {
    ChunkInfo& th = mChunks[i];

    th.offset = endian::native_to_big(th.offset) >> 8;
    endian::native_to_big_inplace(th.time);
    reinterpret_cast<std::uint8_t*>(&th.offset)[3] = th.sectors;

    std::memcpy(buffer + i * 4, &th.offset, sizeof(std::uint32_t));
    std::memcpy(buffer + i * 4 + SectorSize, &th.time, sizeof(std::uint32_t));
  }

  mFile.seekp(0, mFile.beg);
  mFile.write(reinterpret_cast<const char*>(buffer), buf.size());
}

Vector2i AnvilRegion::getRegionCoordsFromChunkCoords(const Vector2i& chcoords) {
  Vector2i x(chcoords.x / 32, chcoords.z / 32);
  if (chcoords.x < 0) {
    --x.x;
  }
  if (chcoords.z < 0) {
    --x.z;
  }

  return x;
}

} // namespace world
} // namespace redi