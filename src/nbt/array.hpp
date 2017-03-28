#ifndef REDI_NBT_ARRAY_HPP
#define REDI_NBT_ARRAY_HPP

#include "basic.hpp"

namespace redi {
namespace nbt {

template <typename T>
struct Array : public BasicTag<Array<T>> {
  using VectorType = std::vector<T>;

  using reference = typename VectorType::reference;
  using const_reference = typename VectorType::const_reference;

  static constexpr Type type = TypeToNumber<Array<T>>::value;

  std::vector<T> data;

  Array() = default;
  Array(const Array&) = default;
  Array(Array&&) = default;

  Array(const VectorType& other) : data(other.data) {}
  Array(VectorType&& other) : data(std::move(other)) {}
  Array(std::size_t count) : data(count) {}
  Array(std::initializer_list<T> list) : data(list) {}

  Array& operator=(const Array& other) {
    *this = other.data;
    return *this;
  }

  Array& operator=(Array&& other) {
    *this = std::move(other.data);
    return *this;
  }

  Array& operator=(const VectorType& other) {
    data = other;
    return *this;
  }

  Array& operator=(VectorType&& other) {
    data = std::move(other);
    return *this;
  }

  Array& operator=(std::initializer_list<T> list) {
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

  void write(Serializer& s) const override {
    s.writeNumber<std::int32_t>(static_cast<std::int32_t>(size()));
    for (const T& index : data) {
      s.writeNumber(index);
    }
  }

  void read(Deserializer& s) override {
    std::size_t siz = static_cast<std::size_t>(s.readNumber<std::int32_t>());
    data.reserve(size() + siz);

    for (std::size_t i = 0; i < siz; ++i) {
      data.emplace_back(s.readNumber<T>());
    }
  }

  void writePretty(PrettyPrint& p) const override {
    Type t = BasicTag<Array<T>>::getType();
    const char* str;

    if (t == Type::ByteArray) {
      str = "byte";
    } else if (t == Type::ShortArray) {
      str = "short";
    } else if (t == Type::IntArray) {
      str = "int";
    } else {
      throw "If this is happen, something went terribly wrong";
    }

    p.string += '[' + std::to_string(size()) + ' ';
    p.string += str;
    if (size() != 1) {
      p.string += 's';
    }
    p.string += ']';
  }
};

using TagByteArray = Array<std::int8_t>;
using TagShortArray = Array<std::int16_t>;
using TagIntArray = Array<std::int32_t>;

} // namespace nbt
} // namespace redi

#endif // REDI_NBT_ARRAY_HPP