#ifndef REDI_NBT_BASIC_HPP
#define REDI_NBT_BASIC_HPP

#include "tag.hpp"

namespace redi
{
namespace nbt
{

template <typename T>
class BasicTag : public Tag
{
public:
  
  virtual ~BasicTag() {}
  
  Type getType() const override { return T::type; }
};
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_BASIC_HPP