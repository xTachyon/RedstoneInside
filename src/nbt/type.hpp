#ifndef REDI_NBT_TYPE
# define REDI_NBT_TYPE

# include <boost/foreach.hpp>
# include <boost/format.hpp>

namespace redi
{
namespace nbt
{

enum class NBTType : std::uint8_t
{
  End,
  Byte,
  Short,
  Int,
  Long,
  Float,
  Double,
  ByteArray,
  String,
  List,
  Compound,
  IntArray
};

const char* getNBTTypeName(NBTType type);

inline std::ostream& operator<<(std::ostream& os, NBTType type)
{
  os << boost::format("NBTType: %1% %2%") % static_cast<int>(type) % getNBTTypeName(type);
  return os;
}

} // namespace nbt
} // namespace redi


#endif // REDI_NBT_TYPE