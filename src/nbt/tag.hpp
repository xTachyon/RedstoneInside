#ifndef REDI_NBT_BASE
#define REDI_NBT_BASE

#include <iostream>
#include <memory>
#include "type.hpp"

namespace redi
{
namespace nbt
{

class Tag
{
public:

  virtual ~Tag() = 0;

  bool isNumber() const;
  bool isScalar() const;
  bool isVector() const;
  bool isContainer() const;
  
  virtual Type getType() const { return Type::End; }

protected:

  virtual bool equals(const Tag&) const { return  false; }
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_BASE