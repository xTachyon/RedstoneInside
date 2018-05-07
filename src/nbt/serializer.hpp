#pragma once

#include <boost/endian/conversion.hpp>
#include "../bytebuffer.hpp"
#include "../util/util.hpp"
#include "forward.hpp"
#include "type.hpp"
#include "visitor.hpp"

namespace redi::nbt {

class serializer: public const_nbt_visitor {
public:
  explicit serializer(ByteBuffer& buffer);

  void visit(const tag_byte& x) override;
  void visit(const tag_short& x) override;
  void visit(const tag_int& x) override;
  void visit(const tag_long& x) override;
  void visit(const tag_float& x) override;
  void visit(const tag_double& x) override;
  void visit(const tag_byte_array& x) override;
  void visit(const tag_string& x) override;
  void visit(const tag_list& list) override;
  void visit(const tag_compound& compound) override;
  void visit(const root_tag& root) override;
  void visit(const tag_int_array& x) override;
  void visit(const tag_long_array& x) override;

  void visit(nbt_string_view name, const tag_compound& root);

  template <typename T>
  void write(T x);
  void write(const nbt_string& string);
  void write(nbt_string_view string);
  void write(tag_type type);
  template <typename Iterator>
  void write(tag_type t, Iterator begin, Iterator end);

private:
  ByteBuffer& buffer;
};

} // namespace redi::nbt