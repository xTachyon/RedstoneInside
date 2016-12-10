#ifndef REDI_NBT_PROXYCLASS
#define REDI_NBT_PROXYCLASS

#include <memory>
#include "tag.hpp"
#include "scalar.hpp"
#include "creator.hpp"
#include "compound.hpp"
#include "string.hpp"
#include "vectorial.hpp"
#include "list.hpp"
#include "prettyprint.hpp"

namespace redi
{
namespace nbt
{

class ProxyClass
{
public:

  ProxyClass(TagPtr& ref, bool canChangeTypes = true)
          : mRef(ref), mCanChangeTypes(canChangeTypes) {}

  ProxyClass& operator=(const Tag& ref)
  {
    // I'll think of something better ...
    if (mRef && ref.getType() == mRef->getType())
      switch (ref.getType())
      {
        case NBTType::Byte:
          mRef->get<NBTType::Byte>() = ref.get<NBTType::Byte>();
          break;

        case NBTType::Short:
          mRef->get<NBTType::Short>() = ref.get<NBTType::Short>();
          break;

        case NBTType::Int:
          mRef->get<NBTType::Int>() = ref.get<NBTType::Int>();
          break;

        case NBTType::Long:
          mRef->get<NBTType::Long>() = ref.get<NBTType::Long>();
          break;

        case NBTType::Float:
          mRef->get<NBTType::Float>() = ref.get<NBTType::Float>();
          break;

        case NBTType::Double:
          mRef->get<NBTType::Double>() = ref.get<NBTType::Double>();
          break;

        case NBTType::ByteArray:
          mRef->get<NBTType::ByteArray>() = ref.get<NBTType::ByteArray>();
          break;

        case NBTType::String:
          mRef->get<NBTType::String>() = ref.get<NBTType::String>();
          break;

        case NBTType::List:
          mRef->get<NBTType::List>() = ref.get<NBTType::List>();
          break;

        case NBTType::Compound:
          mRef->get<NBTType::Compound>() = ref.get<NBTType::Compound>();
          break;

        case NBTType::IntArray:
          mRef->get<NBTType::IntArray>() = ref.get<NBTType::IntArray>();
          break;

        default:
          // TODO: handle end ?
          break;
      }
    else if (mCanChangeTypes) mRef = ref.clone();

    return *this;
  }

  ProxyClass& operator=(TagPtr ptr)
  {
    if (mRef && ptr && mRef->getType() == ptr->getType())
      mRef = ptr;
    else if (mCanChangeTypes) mRef = ptr;

    return *this;
  }

  ProxyClass& operator=(std::int8_t data)
  {
    *this = TagByte(data);
    return *this;
  }

  ProxyClass& operator=(std::int16_t data)
  {
    *this = TagShort(data);
    return *this;
  }

  ProxyClass& operator=(std::int32_t data)
  {
    *this = TagInt(data);
    return *this;
  }

  ProxyClass& operator=(std::int64_t data)
  {
    *this = TagLong(data);
    return *this;
  }

  ProxyClass& operator=(float data)
  {
    *this = TagFloat(data);
    return *this;
  }

  ProxyClass& operator=(double data)
  {
    *this = TagDouble(data);
    return *this;
  }

  ProxyClass& operator=(const std::string& data)
  {
    *this = TagString(data);
    return *this;
  }

  ProxyClass& operator=(std::string&& data)
  {
    *this = TagString(std::move(data));
    return *this;
  }

  ProxyClass operator=(const std::vector<std::int8_t>& data)
  {
    *this = TagByteArray(data);
    return *this;
  }

  ProxyClass operator=(std::vector<std::int8_t>&& data)
  {
    *this = TagByteArray(std::move(data));
    return *this;
  }

  ProxyClass operator=(std::vector<std::int32_t>&& data)
  {
    *this = TagIntArray(std::move(data));
    return *this;
  }

  ProxyClass operator[](const std::string& index)
  {
    return (*this)[std::string(index)];
  }

  ProxyClass operator[](std::string&& index)
  {
    if (!mRef || mRef->getType() != NBTType::Compound) mRef = create(NBTType::Compound);

    return mRef->get<TagCompound>()[std::move(index)];
  }

  Tag& get() { if (mRef) return *mRef; throw 1; }
  const Tag& get() const { if (mRef) return *mRef; throw 1; }

  operator Tag&() { if (mRef) return *mRef; throw 1; }
  operator const Tag&() const { if (mRef) return *mRef; throw 1; }
  // TODO: throw something better

  operator bool() const { return static_cast<bool>(mRef); }

  friend std::ostream& operator<<(std::ostream& stream, const ProxyClass& obj)
  {
    if (obj)
    {
      if (obj.mRef->getType() == NBTType::Compound && dynamic_cast<const RootTag*>(obj.mRef.get()) != nullptr) stream << PrettyPrint(dynamic_cast<const RootTag&>(*obj.mRef.get()));
      else stream << "only root tags can be printed";
    }
    else stream << "null";

    return stream;
  }


private:

  TagPtr& mRef;
  bool mCanChangeTypes;
};

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_PROXYCLASS