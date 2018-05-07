#include "value.hpp"
#include "compound.hpp"
#include "serializer.hpp"
#include "roottag.hpp"
#include "nbt.hpp"

namespace redi::nbt {

serializer::serializer(ByteBuffer& buf)
      : buffer(buf) {}

void serializer::visit(const tag_byte& x) {
  write(x.data);
}

void serializer::visit(const tag_short& x) {
  write(x.data);
}

void serializer::visit(const tag_int& x) {
  write(x.data);
}

void serializer::visit(const tag_long& x) {
  write(x.data);
}

void serializer::visit(const tag_float& x) {
  write(util::binaryTo<nbt_float, nbt_int>(x.data));
}

void serializer::visit(const tag_double& x) {
  write(util::binaryTo<nbt_double, nbt_long>(x.data));
}

void serializer::visit(const tag_byte_array& array) {
  write(array.get_type(), array.data.begin(), array.data.end());
}

void serializer::visit(const tag_string& x) {
  write(x.data);
}

void serializer::visit(const tag_list& list) {
  tag_type type = list.getListType();
  auto& data = list.getData();

  write(type);
  write(static_cast<nbt_int>(data.size()));

  for (auto& index : data) {
    index.visit(*this);
  }
}

void serializer::visit(const tag_compound& compound) {
  for (auto& index : compound) {
    auto& name = index.first;
    auto& tag = index.second;

    tag_type type = tag.get_type();

    write(type);
    write(name);
    if (type != tag_type::type_end) {
      tag.visit(*this);
    }
  }

  write(tag_type::type_end);
}

void serializer::visit(const root_tag& tag) {
  visit(tag.name, tag.compound);
}

void serializer::visit(const tag_int_array& array) {
  write(array.get_type(), array.data.begin(), array.data.end());
}

void serializer::visit(const tag_long_array& array) {
  write(array.get_type(), array.data.begin(), array.data.end());
}

void serializer::visit(nbt_string_view name, const tag_compound& root) {
  write(tag_type::type_compound);
  write(name);
  visit(root);
}

template <typename T>
void serializer::write(T x) {
  // Yes I can do that
  boost::endian::native_to_big_inplace(x);
  buffer.append(x);
}


void serializer::write(const nbt_string& string) {
  write(nbt_string_view(string));
}

void serializer::write(nbt_string_view string) {
  write(static_cast<std::uint16_t>(string.size()));
  buffer.append(string.data(), string.size());
}

void serializer::write(tag_type type) {
  write(static_cast<nbt_byte>(type));
}

template <typename Iterator>
void serializer::write(tag_type t, Iterator begin, Iterator end) {
  write(static_cast<nbt_int>(end - begin));
  std::for_each(begin, end, [this](const auto& x) {
    write(x);
  });
}

} // namespace redi::nbt