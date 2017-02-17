#ifndef REDI_NBT_ARRAY_HPP
#define REDI_NBT_ARRAY_HPP

#include "basic.hpp"

namespace redi
{
namespace nbt
{

template <typename T>
struct Array : public BasicTag<Array<T>>
{
  using VectorType = std::vector<T>;
  
  using reference = typename VectorType::reference;
  using const_reference = typename VectorType::const_reference;

  static constexpr Type type = TypeToNumber<Array<T>>::value;
  
  std::vector<T> data;
  
  Array() = default;
  Array(const Array&) = default;
  Array(Array&&) = default;
  
  Array(const VectorType& other) : data(other.data) {}
  Array(VectorType&& other) : data(std::move(other.data)) {}
  Array(std::size_t count) : data(count) {}
  Array(std::initializer_list<T> list) : data(list) {}
  
  Array& operator=(const Array& other)
  {
    *this = other.data;
    return *this;
  }
  
  Array& operator=(Array&& other)
  {
    *this = std::move(other.data);
    return *this;
  }
  
  Array& operator=(const VectorType& other)
  {
    data = other.data;
    return *this;
  }
  
  Array& operator=(VectorType&& other)
  {
    data = std::move(other.data);
    return *this;
  }
  
  Array& operator=(std::initializer_list<T> list)
  {
    data = list;
    return *this;
  }
  
  reference operator[](std::size_t index)
  {
    return data[index];
  }
  
  const_reference operator[](std::size_t index) const
  {
    return data[index];
  }
  
  reference at(std::size_t index)
  {
    return at(index);
  }
  
  const_reference at(std::size_t index) const
  {
    return at(index);
  }
  
  reference front()
  {
    return data.front();
  }
  
  const_reference front() const
  {
    return data.front();
  }
  
  reference back()
  {
    return data.back();
  }
  
  const_reference back() const
  {
    return data.back();
  }
};

using TagByteArray = Array<std::int8_t>;
using TagIntArray = Array<std::int32_t>;
  
} // namespace nbt
} // namespace redi

#endif // REDI_NBT_ARRAY_HPP