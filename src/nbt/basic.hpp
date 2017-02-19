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
  
  Tag& assign(Tag&& tag) override { return der() = dynamic_cast<T&&>(tag); }
  Tag& assign(const Tag& tag) override { return der() = dynamic_cast<const T&>(tag); }
  
  Type getType() const override { return T::type; }
  
  TagUniquePtr clone() const & override { return std::make_unique<T>(der()); }
  TagUniquePtr move() && override { return std::make_unique<T>(std::move(der())); }
  
private:
  
  T& der() { return static_cast<T&>(*this); }
  const T& der() const { return static_cast<const T&>(*this); }
};
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_BASIC_HPP