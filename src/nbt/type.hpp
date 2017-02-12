#ifndef REDI_NBT_TYPE
# define REDI_NBT_TYPE

#include <boost/format.hpp>

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

} // namespace nbt
} // namespace redi


#endif // REDI_NBT_TYPE