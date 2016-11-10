#ifndef REDI_NBT_STDSPECIALIZATIONS
# define REDI_NBT_STDSPECIALIZATIONS

# include <algorithm>
# include "tag.hpp"
# include "scalar.hpp"
# include "string.hpp"
# include "compound.hpp"
# include "list.hpp"

namespace std
{

template <>
void swap<redi::nbt::TagCompound>(redi::nbt::TagCompound& l, redi::nbt::TagCompound& r)
{
  l.swap(r);
}

template <>
void swap<redi::nbt::TagList>(redi::nbt::TagList& l, redi::nbt::TagList& r)
{
  l.swap(r);
}

template <>
void swap<redi::nbt::TagString>(redi::nbt::TagString& l, redi::nbt::TagString& r)
{
  l.swap(r);
}

} // namespace std

#endif // REDI_NBT_STDSPECIALIZATIONS