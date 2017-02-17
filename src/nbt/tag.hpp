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
  
  virtual Tag& assign(Tag&& tag) = 0;
  
  virtual Type getType() const { return Type::End; }
  
  virtual std::unique_ptr<Tag> clone() const & = 0;
  virtual std::unique_ptr<Tag> move() && = 0;

protected:

  virtual bool equals(const Tag&) const { return  false; }
};


using TagUniquePtr = std::unique_ptr<Tag>;

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_BASE