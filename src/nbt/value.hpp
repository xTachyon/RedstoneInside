#ifndef REDI_NBT_VALUE_HPP
#define REDI_NBT_VALUE_HPP

#include <memory>
#include "tag.hpp"

namespace redi
{
namespace nbt
{

class Value
{
public:
  
private:
  
  std::unique_ptr<Tag> mData;
};
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_VALUE_HPP