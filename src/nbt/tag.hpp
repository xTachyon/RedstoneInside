#ifndef REDI_NBT_BASE
# define REDI_NBT_BASE

# include <iostream>
# include <memory>
# include "type.hpp"
# include "compiletimemagic.hpp"

namespace redi
{
namespace nbt
{

class Tag
{
public:

  Tag(NBTType type);

  virtual ~Tag() = 0;

  NBTType getType() const { return mType; }

  friend bool operator==(const Tag& l, const Tag& r);

  virtual std::shared_ptr<Tag> clone() const = 0; // { return nullptr; }

  template <typename T>
  T& get()
  {
    if (mType == T::type) return dynamic_cast<T&>(*this);
    throw std::invalid_argument("");
  }

  template <typename T>
  const T& get() const
  {
    if (mType == T::type) return dynamic_cast<const T&>(*this);
    throw std::invalid_argument("");
  }

  template <NBTType type>
  typename TagTypeInfoType<type>::returnType& get()
  {
    return get<typename TagTypeInfoType<type>::castType>();
  }

  template <NBTType type>
  const typename TagTypeInfoType<type>::returnType& get() const
  {
    return get<typename TagTypeInfoType<type>::castType>();
  }

  bool isNumeric() const;
  bool isScalar() const;
  bool isVector() const;
  bool isComplex() const; // find a better name

protected:

  NBTType mType;

  virtual bool equals(const Tag& r) const = 0;
};

bool operator!=(const Tag& l, const Tag& r);

using TagPtr = std::shared_ptr<Tag>;

} // namespace nbt
} // namespace redi

#endif