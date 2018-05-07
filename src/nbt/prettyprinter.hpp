#pragma once

#include <string>
#include <boost/format.hpp>
#include "type.hpp"
#include "visitor.hpp"

namespace redi::nbt {

struct pretty_print_options {
  nbt_int indent_size;
  nbt_string indent_character;

  explicit pretty_print_options(nbt_int indent_size = 2, nbt_string indent_character = " ");
};

class pretty_printer : public const_nbt_visitor {
public:
  std::string string;

  explicit pretty_printer(nbt_int indent = 2);
  explicit pretty_printer(pretty_print_options options);

  void visit(const tag_byte& x) override;
  void visit(const tag_short& x) override;
  void visit(const tag_int& x) override;
  void visit(const tag_long& x) override;
  void visit(const tag_float& x) override;
  void visit(const tag_double& x) override;
  void visit(const tag_byte_array& x) override;
  void visit(const tag_string& x) override;
  void visit(const tag_list& x) override;
  void visit(const tag_compound& x) override;
  void visit(const root_tag& x) override;
  void visit(const tag_int_array& x) override;
  void visit(const tag_long_array& x) override;

  pretty_print_options options;
  std::string indent_string;

  void writeType(tag_type t);
  void writeType(tag_type t, nbt_string_view str);
  void writeIndent(nbt_int update = 0);
  void writeNewLine();
  void writeEntry(std::size_t size);
  void writeOne(const tag& tag);
  void writeArrayNumbers(nbt_string_view name, std::size_t size);

  void append(nbt_string_view str);
  void append(const boost::format& format);

  template <typename T>
  void writeNumber(T x);
};

std::ostream& operator<<(std::ostream& stream, const pretty_printer& printer);

} // namespace redi::nbt