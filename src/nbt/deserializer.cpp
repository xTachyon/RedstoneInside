#include "compound.hpp"
#include "roottag.hpp"
#include "deserializer.hpp"
#include "nbt.hpp"

namespace redi::nbt {

deserializer::deserializer(const ByteBuffer& buffer, size_t offset)
      : buffer(buffer), offset(offset) {}

void deserializer::visit(tag_end&) {
}

void deserializer::visit(tag_byte& x) {
  readNumber(x.data);
}

void deserializer::visit(tag_short& x) {
  readNumber(x.data);
}

void deserializer::visit(tag_int& x) {
  readNumber(x.data);
}

void deserializer::visit(tag_long& x) {
  readNumber(x.data);
}

void deserializer::visit(tag_float& x) {
  readNumber(x.data);
}

void deserializer::visit(tag_double& x) {
  readNumber(x.data);
}

void deserializer::visit(tag_byte_array& array) {
  readArray(array.data);
}

void deserializer::visit(tag_string& string) {
  string.data = readString();
}

void deserializer::visit(tag_list& list) {
  tag_type t = readType();
  auto& data = list.getData();
  auto size = static_cast<std::size_t>(readNumber<std::int32_t>());
  data.reserve(size);

  for (std::size_t i = 0; i < size; ++i) {
    tag_unique_ptr tag = create(t);
    if (t != tag_type::type_end) {
      tag->visit(*this);
    }
    data.emplace_back(std::move(tag));
  }
}

void deserializer::visit(tag_compound& compound) {
  tag_type type;
  nbt_string name;

  while ((type = readType()) != tag_type::type_end) {
    name = readString();
    tag_unique_ptr tag = create(type);
    if (type != tag_type::type_end) {
      tag->visit(*this);
    }
    compound.emplace(std::move(name), std::move(tag));
  }
}

void deserializer::visit(root_tag& root) {
  visit(root.name, root.compound);
}

void deserializer::visit(tag_int_array& array) {
  readArray(array.data);
}

void deserializer::visit(tag_long_array& array) {
  readArray(array.data);
}

void deserializer::visit(nbt_string& name, tag_compound& root) {
  tag_type type = readType();
  if (type != tag_type::type_compound) {
    throw std::runtime_error("Root tag must be compound");
  }
  name = readString();
  visit(root);
}

template <typename T>
T deserializer::readNumber() {
  need(sizeof(T));
  T x;

  std::copy(buffer.data() + offset, buffer.data() + offset + sizeof(T),
            reinterpret_cast<std::uint8_t*>(std::addressof(x)));
  offset += sizeof(T);
  return boost::endian::big_to_native(x);
}

template <>
float deserializer::readNumber<float>() {
  std::int32_t x = readNumber<std::int32_t>();
  return util::binaryTo<std::int32_t, float>(x);
}

template <>
double deserializer::readNumber<double>() {
  std::int64_t x = readNumber<std::int64_t>();
  return util::binaryTo<std::int64_t, double>(x);
}

nbt_string deserializer::readString() {
  std::size_t size = readNumber<std::uint16_t>();
  need(size);
  auto ptr = buffer.as_const_char() + offset;
  std::string s(ptr, ptr + size);
  offset += size;
  return s;
}

tag_type deserializer::readType() {
  need(sizeof(tag_type));
  std::uint8_t x = buffer[offset++];
  if (x > 12) {
    throw std::runtime_error(std::to_string(static_cast<int>(x)) +
                             " is an unrecognized NBT tag");
  }
  return static_cast<tag_type>(x);
}

void deserializer::need(std::size_t bytes) {
  if (bytes + offset > buffer.size()) {
    throw std::runtime_error(std::to_string(bytes) + " is out of index");
  }
}

template <typename T>
void deserializer::readNumber(T& x) {
  x = readNumber<T>();
}

template<typename T>
void deserializer::readArray(std::vector<T>& data) {
  auto size = static_cast<std::size_t>(readNumber<std::int32_t>());
  data.reserve(data.size() + size);

  for (std::size_t i = 0; i < size; ++i) {
    data.emplace_back(readNumber<T>());
  }
}

} // namespace redi::nbt