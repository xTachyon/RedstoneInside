#ifndef REDI_NBT_FORWARD_HPP
#define REDI_NBT_FORWARD_HPP

#include <cstdint>

namespace redi
{
namespace nbt
{

class Tag;

template <typename T>
class BasicTag;

template <typename T>
struct Primitive;

template <typename T>
struct Array;

using TagByte = Primitive<std::int8_t>;
using TagShort = Primitive<std::int16_t>;
using TagInt = Primitive<std::int32_t>;
using TagLong = Primitive<std::int64_t>;
using TagFloat = Primitive<float>;
using TagDouble = Primitive<double>;

using TagByteArray = Array<std::int8_t>;
using TagIntArray = Array<std::int32_t>;

struct TagString;
class TagCompound;
class TagList;
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_FORWARD_HPP