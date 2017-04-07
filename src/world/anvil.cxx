#include <boost/filesystem.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/endian/arithmetic.hpp>
#include "anvil.hpp"
#include "../logger.hpp"
#include "../compressor.hpp"
#include "../util/util.hpp"

namespace fs = boost::filesystem;
namespace endian = boost::endian;

namespace redi {
namespace world {

Anvil::Anvil() : hasRegion(false) {}

Anvil::Anvil(const std::string& path, const Vector2i& pos, bool create)
    : Anvil(fs::path(path), pos, create) {}

Anvil::Anvil(const boost::filesystem::path& path, const Vector2i& pos,
             bool create) {
  open(path, pos, create);
}

Anvil::~Anvil() { close(); }

void Anvil::open(const boost::filesystem::path& path, const Vector2i& pos,
                 bool create) {
  clear();
  position = pos;

  createNewRegionIfOK(path, create);

  auto filesize = fs::file_size(path);

  if (filesize % 4_KB != 0 || filesize < HeaderSize) {
    fs::path newpath(path.string() + ".backup");
    Logger::info((boost::format("%1% has an invalid size(%2% bytes). Renaming "
                                "to %3% and trying to create a new one") %
                  path % filesize % newpath)
                     .str());

    fs::rename(path, newpath);
    createNewRegionIfOK(path, create);
  } else {
    openFile(path.string());
  }

  hasRegion = true;
  freeSectors.resize(static_cast<std::size_t>(filesize / SectorSize));
  readHeader();
  processHeader();
}

void Anvil::close() {
  if (hasRegion) {
    flush();
    clear();
  }
}

void Anvil::flush() {
  file.seekp(0, file.beg);
  file.write(reinterpret_cast<const char*>(header.data()), header.size());
}

void Anvil::clear() {
  file.close();
  hasRegion = false;
  freeSectors.clear();
}

void Anvil::openFile(const std::string& path) {
  file.open(path, std::ios::in | std::ios::out | std::ios::binary);
  file.exceptions(std::ios::failbit | std::ios::badbit);
}

void Anvil::createNewRegionAndOpen(const std::string& filepath) {
  file.open(filepath,
            std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
  file.exceptions(std::ios::failbit | std::ios::badbit);
  //  openFile(filepath);
  std::fill(header.begin(), header.end(), 0);
  flush();
  file.close();
}

std::int32_t Anvil::getChunkNumberInRegion(const Vector2i& other) {
   return (other.x & 31) + (other.z & 31) * 32;
  //  return 4 * ((other.x & 31) + (other.z & 31) * 32);
}

Vector2i Anvil::getRegionCoordsFromChunkCoords(const Vector2i& chcoords) {
  Vector2i x(chcoords.x / 32, chcoords.z / 32);
  if (chcoords.x < 0) {
    --x.x;
  }
  if (chcoords.z < 0) {
    --x.z;
  }

  return x;
}

void Anvil::createNewRegionIfOK(const fs::path& filepath, bool create) {
  if (!fs::exists(filepath)) {
    if (!create) {
      throw std::runtime_error(
          "Region file doesn't exists and set to not create: " +
          filepath.string());
    }
    createNewRegionAndOpen(filepath.string());
  }
}

void Anvil::readHeader() {
  file.seekg(0, file.beg);
  file.read(reinterpret_cast<char*>(header.data()), header.size());
}

void Anvil::processHeader() {
  freeSectors[0] = freeSectors[1] = false;

  for (std::int32_t i = 0; i < ChunksPerRegion; ++i) {
    ChunkInfo info = getChunkInfo(i);

    if (info.offset + info.sectors > freeSectors.size()) {
      Logger::warn(
          (boost::format("Chunk number %1% is out of range. Free sectors size "
                         ": %2%. info.offset: %3%. info.sectors: %4%") %
           i % freeSectors.size() % info.offset % info.sectors)
              .str());
      writeChunkInfo(i, ChunkInfo());
      continue;
    }
    for (std::size_t j = info.offset, max = info.offset + info.sectors; j < max;
         ++j) {
      freeSectors[j] = false;
    }
  }
}

Anvil::ChunkInfo Anvil::getChunkInfo(std::int32_t number) const {
  assert(0 <= number && number < ChunksPerRegion);

  ChunkInfo info;

  info.sectors = header[number * 4 + 3];

  {
    auto begin = header.begin() + static_cast<std::size_t>(number * 4);
    auto end = begin + sizeof(info.offset) - 1;
    endian::big_uint24_t x;
    std::copy(begin, end, reinterpret_cast<std::uint8_t*>(&x));

    info.offset = static_cast<std::uint32_t>(x);
  }

  {
    auto begin = header.begin() + SectorSize + number * 4;
    auto end = begin + sizeof(info.time);
    std::copy(begin, end, reinterpret_cast<std::uint8_t*>(&info.time));

    endian::big_to_native_inplace(info.time);
  }

  return info;
}

Anvil::ChunkInfo Anvil::getChunkInfo(const Vector2i& chunk) const {
  return getChunkInfo(getChunkNumberInRegion(chunk));
}

void Anvil::writeChunkInfo(const Vector2i& number,
                           const Anvil::ChunkInfo& chunk) {
  writeChunkInfo(getChunkNumberInRegion(number), chunk);
}

void Anvil::writeChunkInfo(std::int32_t number, const Anvil::ChunkInfo& info) {
  header[number * 4 + 3] = info.sectors;

  {
    endian::big_uint24_t x = info.offset;

    auto begin = reinterpret_cast<const std::uint8_t*>(&x);
    auto end = begin + sizeof(info.offset) - 1;
    std::copy(begin, end, header.begin() + number * 4);
  }

  {
    auto x = endian::native_to_big(info.time);

    auto begin = reinterpret_cast<const std::uint8_t*>(&x);
    auto end = begin + sizeof(info.time);
    std::copy(begin, end, header.begin() + SectorSize + number * 4);
  }

  std::ofstream("a.b").write(reinterpret_cast<const char*>(header.data()),
                             header.size());
}

Anvil::ChunkReadResult Anvil::readChunk(const Vector2i& number,
                                        ByteBuffer& buffer) {
  return readChunk(getChunkNumberInRegion(number), buffer);
}

void Anvil::writeChunk(const Vector2i& number,
                       const ByteBuffer& uncompresseddata) {
  writeChunk(getChunkNumberInRegion(number), uncompresseddata);
}

Anvil::ChunkReadResult Anvil::readChunk(std::int32_t number,
                                        ByteBuffer& buffer) {
#ifdef REDI_DEBUG
  if (!(0 <= number && number < 1024)) {
  }
#endif

  ChunkInfo info = getChunkInfo(number);

  if (info.offset == 0 || info.sectors == 0) {
    return ChunkReadResult::DoesntExists;
  }

  buffer.resize(SectorSize * info.sectors);
  file.seekg(info.offset * SectorSize);
  file.read(buffer.as_char(), buffer.size());

  if (file.gcount() < 5) {
    Logger::error(std::to_string(file.gcount()) +
                  " bytes read in Anvil.read. Expected at least 5");
    return ChunkReadResult::Error;
  }

  std::uint32_t size;
  std::copy(buffer.data(), buffer.data() + sizeof(size),
            reinterpret_cast<std::uint8_t*>(&size));
  endian::big_to_native_inplace(size);

  if (size < 5) {
    Logger::error(std::to_string(size) +
                  " bytes marked in chunk in Anvil.read. Expected at least 5");
    return ChunkReadResult::Error;
  }

  switch (buffer[4]) {
  case 1: {
    // TODO: fix compressor and add decompressor
  } break;

  case 2: {
    buffer = compressor::decompressZlib(buffer, 5, size - 1);
  } break;

  default: {
    Logger::error(std::to_string(static_cast<int>(buffer[4])) +
                  " - unknown compression in Anvil.read. Expected 1 or 2.");
    return ChunkReadResult::Error;
  }
  }

  return ChunkReadResult::OK;
}

void Anvil::writeChunk(std::int32_t number,
                       const ByteBuffer& uncompresseddata) {
  constexpr std::size_t sizetmax = std::numeric_limits<std::size_t>::max();

#ifdef REDI_DEBUG
  if (!(0 <= number && number < 1024)) {
  }
#endif

  ByteBuffer buffer;

  {
    buffer.reserve(SectorSize);
    buffer.resize(5);
  }

  compressor::compressZlib(uncompresseddata, buffer,
                           compressor::CompressionLevel::BestCompression);
  std::size_t datasize = buffer.size();

  ChunkInfo info = getChunkInfo(number);
  std::size_t newSectorCount = getSectorsNumber(datasize);

  if (newSectorCount == info.sectors) {
    /*
    * If the actual sector count is equal
    * with the old one, just overwrite.
    */
    file.seekp(info.offset * SectorSize);
  } else {
    for (std::size_t i = info.offset; i < info.offset + info.sectors; ++i) {
      freeSectors[i] = true;
      // Mark old sectors as free
    }

    std::size_t start = sizetmax;
    std::size_t len = 0;

    for (std::size_t i = 0; i < freeSectors.size(); ++i) {
      if (freeSectors[i]) {
        start = i;
        for (; i < freeSectors.size() && freeSectors[i]; ++i)
          ;
        len = i - start;
        if (len == newSectorCount)
          break;
        // Perfect
      }
    }

    if (start == sizetmax) {
      // If no good sectors were found
      file.seekp(0, file.end);
      // Set to end of file
      buffer.resize(datasize + SectorSize - datasize % SectorSize);
      // Make it 4 kb divisible
      info.offset = static_cast<std::uint32_t>(freeSectors.size());
      // Update the information about this chunk in the header
      for (std::size_t i = 0; i < info.sectors; ++i) {
        freeSectors.push_back(false);
      }
    } else {
      // Good sectors were found.
      file.seekp(start * SectorSize);
      info.offset = static_cast<std::uint32_t>(start);
    }

    info.sectors = static_cast<std::uint8_t>(newSectorCount);
  }

  info.time = static_cast<std::uint32_t>(util::getUnixTimestamp());

  {
    std::uint32_t x =
        endian::native_to_big(static_cast<std::uint32_t>(datasize - 4));

    auto ptr = reinterpret_cast<const std::uint8_t*>(&x);
    std::copy(ptr, ptr + sizeof(x), buffer.data());
    // size

    buffer[4] = 2;
    // compression
  }

  file.write(buffer.as_const_char(), buffer.size());
  writeChunkInfo(number, info);
}

size_t Anvil::getSectorsNumber(std::size_t size) {
  std::size_t ret = size / SectorSize;

  if (size % SectorSize != 0) {
    ++ret;
  }

  return ret;
}

} // namespace world
} // namespace redi