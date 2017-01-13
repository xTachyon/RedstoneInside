#ifndef REDI_NBT_STDSPECIALIZATIONS
#define REDI_NBT_STDSPECIALIZATIONS

#include <algorithm>
#include "tag.hpp"
#include "scalar.hpp"
#include "string.hpp"
#include "compound.hpp"
#include "list.hpp"
#include "vectorial.hpp"

namespace std
{

template <>
inline void swap<redi::nbt::TagCompound>(redi::nbt::TagCompound& l, redi::nbt::TagCompound& r)
{
  l.swap(r);
}

template <>
inline void swap<redi::nbt::TagList>(redi::nbt::TagList& l, redi::nbt::TagList& r)
{
  l.swap(r);
}

template <>
inline void swap<redi::nbt::TagString>(redi::nbt::TagString& l, redi::nbt::TagString& r) noexcept
{
  l.swap(r);
}

template <>
inline void swap<redi::nbt::TagByteArray>(redi::nbt::TagByteArray& l, redi::nbt::TagByteArray& r) noexcept
{
  l.data.swap(r.data);
}

template <>
inline void swap<redi::nbt::TagIntArray>(redi::nbt::TagIntArray& l, redi::nbt::TagIntArray& r) noexcept
{
  l.data.swap(r.data);
}

} // namespace std

#endif // REDI_NBT_STDSPECIALIZATIONS