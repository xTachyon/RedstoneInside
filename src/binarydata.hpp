#ifndef REDI_BINARYDATA
#define REDI_BINARYDATA

#include <cstdint>
#include <vector>

namespace redi
{

class BinaryData : public std::vector<std::uint8_t>
{
public:

  using base = std::vector<std::uint8_t>;

  using base::base;
  using base::operator=;
  using base::operator[];

  BinaryData() = default;

  BinaryData(const std::uint8_t* ptr, base::size_type size)
  {
    append(ptr, size);
  }

  void append(const std::uint8_t* ptr, base::size_type size)
  {
    insert(end(), ptr, ptr + size);
  }

  void append(const char* ptr, base::size_type size)
  {
    append(reinterpret_cast<const std::uint8_t*>(ptr), size);
  }
};

} // namespace redi

#endif // REDI_BINARYDATA