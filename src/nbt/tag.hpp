#ifndef REDI_NBT_BASE
#define REDI_NBT_BASE

#include <iostream>
#include <memory>
#include "type.hpp"
#include "forward.hpp"

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
  virtual std::size_t size() const { return 1; }
  
  virtual Tag& assign(Tag&& tag) = 0;
  virtual Tag& assign(const Tag& tag) = 0;
  
  virtual Type getType() const { return Type::End; }
  
  virtual std::unique_ptr<Tag> clone() const & = 0;
  virtual std::unique_ptr<Tag> move() && = 0;

  virtual bool equals(const Tag&) const { return false; }
  
  virtual void write(Serializer&) const = 0;
  virtual void read(Deserializer&) = 0;
  virtual void writePretty(PrettyPrint&) const {}
};

std::ostream& operator<<(std::ostream& stream, const Tag& tag);

using TagUniquePtr = std::unique_ptr<Tag>;

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_BASE