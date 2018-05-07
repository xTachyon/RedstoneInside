#pragma once

#include <boost/endian/conversion.hpp>
#include "../bytebuffer.hpp"
#include "type.hpp"
#include "../util/string.hpp"

namespace redi::nbt {

class deserializer : public nbt_visitor {
public:
  explicit deserializer(const ByteBuffer& buffer, std::size_t offset = 0);

  void visit(tag_end& x) override;
  void visit(tag_byte& x) override;
  void visit(tag_short& x) override;
  void visit(tag_int& x) override;
  void visit(tag_long& x) override;
  void visit(tag_float& x) override;
  void visit(tag_double& x) override;
  void visit(tag_byte_array& array) override;
  void visit(tag_string& string) override;
  void visit(tag_list& list) override;
  void visit(tag_compound& compound) override;
  void visit(root_tag& tag) override;
  void visit(tag_int_array& array) override;
  void visit(tag_long_array& array) override;

  void visit(nbt_string& name, tag_compound& compound);

private:
  const ByteBuffer& buffer;
  std::size_t offset;

  template <typename T>
  T readNumber();
  template <typename T>
  void readNumber(T& x);
  template <typename T>
  void readArray(std::vector<T>& array);
  nbt_string readString();
  tag_type readType();

  void need(std::size_t bytes);
};

} // namespace redi::nbt