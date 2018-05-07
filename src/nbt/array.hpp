#pragma once

#include "basic.hpp"
#include "forward.hpp"

namespace redi::nbt {

template <typename T>
struct array : public basic_tag<array<T>> {
  using array_container = std::vector<T>;

  using reference = typename array_container::reference;
  using const_reference = typename array_container::const_reference;

  static constexpr tag_type type = type_to_number<array<T>>::value;

  array_container data;

  array() = default;
  array(const array&) = default;
  array(array&&) noexcept = default;

  explicit array(const array_container& other) : data(other.data) {}

  explicit array(array_container&& other) : data(std::move(other)) {}
  explicit array(std::size_t count) : data(count) {}
  array(std::initializer_list<T> list) : data(list) {}

  array& operator=(const array& other) noexcept {
    *this = other.data;
    return *this;
  }

  array& operator=(array&& other) noexcept {
    *this = std::move(other.data);
    return *this;
  }

  array& operator=(const array_container& other) {
    data = other;
    return *this;
  }

  array& operator=(array_container&& other) {
    data = std::move(other);
    return *this;
  }

  array& operator=(std::initializer_list<T> list) {
    data = list;
    return *this;
  }

  reference operator[](std::size_t index) { return data[index]; }
  const_reference operator[](std::size_t index) const { return data[index]; }

  reference at(std::size_t index) { return at(index); }
  const_reference at(std::size_t index) const { return at(index); }

  reference front() { return data.front(); }
  const_reference front() const { return data.front(); }

  reference back() { return data.back(); }
  const_reference back() const { return data.back(); }

  std::size_t size() const override { return data.size(); }
};

using tag_byte_array_container = tag_byte_array::array_container;
using tag_int_array_container = tag_int_array::array_container;
using tag_long_array_container = tag_long_array::array_container;

} // namespace redi::nbt