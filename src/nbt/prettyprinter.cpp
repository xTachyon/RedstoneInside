#include <boost/format.hpp>
#include <utility>
#include "prettyprinter.hpp"
#include "tag.hpp"
#include "nbt.hpp"

namespace redi::nbt {

pretty_print_options::pretty_print_options(nbt_int indent_size, nbt_string indent_character)
      : indent_size(indent_size), indent_character(std::move(indent_character)) {}

pretty_printer::pretty_printer(pretty_print_options options)
      : options(std::move(options)) {}

pretty_printer::pretty_printer(nbt_int indent)
      : pretty_printer(pretty_print_options(indent)) {}

struct IndentBlock {
  pretty_printer& printer;

  explicit IndentBlock(pretty_printer& printer);
  ~IndentBlock();
};

IndentBlock::IndentBlock(pretty_printer& printer)
      : printer(printer) {
  printer.writeIndent(printer.options.indent_size);
}

IndentBlock::~IndentBlock() {
  printer.writeIndent(-printer.options.indent_size);
}

template <typename T>
void pretty_printer::writeNumber(T x) {
  append(std::to_string(x));
}

void pretty_printer::visit(const tag_byte& x) {
  writeNumber(static_cast<int>(x.data));
}

void pretty_printer::visit(const tag_short& x) {
  writeNumber(x.data);
}

void pretty_printer::visit(const tag_int& x) {
  writeNumber(x.data);
}

void pretty_printer::visit(const tag_long& x) {
  writeNumber(x.data);
}

void pretty_printer::visit(const tag_float& x) {
  writeNumber(x.data);
}

void pretty_printer::visit(const tag_double& x) {
  writeNumber(x.data);
}

void pretty_printer::visit(const tag_byte_array& x) {
  writeArrayNumbers("byte", x.size());
}

void pretty_printer::visit(const tag_string& x) {
  append(boost::format("%2%%1%%2%") % x.data % '\"');
}

void pretty_printer::visit(const tag_list& x) {
  IndentBlock indent_block(*this);
  tag_type t = x.getListType();

  for (auto& index : x.get()) {
    // TODO: fix with range loop on tag_list
    writeNewLine();
    writeIndent();

    writeType(t);
    if (index) {
      writeOne(index.get());
    }

    index.get().visit(*this);
  }
}

void pretty_printer::visit(const tag_compound& x) {
  IndentBlock indent_block(*this);

  for (auto& index : x) {
    writeNewLine();
    writeIndent();

    tag_type type = index.second.get_type();
    writeType(type, index.first);

    auto& second_tag = index.second.get();
    if (index.second) {
      writeOne(second_tag);
    }

    second_tag.visit(*this);
  }
}

void pretty_printer::visit(const tag_int_array& x) {
  writeArrayNumbers("int", x.size());
}

void pretty_printer::visit(const tag_long_array& x) {
  writeArrayNumbers("long", x.size());
}

void pretty_printer::writeType(tag_type t, nbt_string_view str) {
  append(boost::format("%1%(\"%2%\") : ") % getNBTTypeName(t) % str);
}

void pretty_printer::append(nbt_string_view str) {
  string += str;
}

void pretty_printer::writeIndent(nbt_int i) {
  // TODO: fix printing indent at the end of the line
  if (i > 0) {
    for (; i > 0; --i) {
      indent_string += options.indent_character;
    }
  } else if (i < 0) {
    i *= options.indent_character.size();
    for (; i < 0; ++i) {
      indent_string.pop_back();
    }
  }

  append(indent_string);
}

void pretty_printer::writeNewLine() {
  append("\n");
}

void pretty_printer::writeEntry(std::size_t size) {
  append(std::to_string(size) + (size == 1 ? " entry" : " entries"));
}

void pretty_printer::visit(const root_tag& x) {
  writeType(x.type, x.name);
  writeOne(x.compound);
  visit(x.compound);
}

void pretty_printer::writeOne(const tag& tag) {
  if (tag.is_container()) {
    writeEntry(tag.size());
  }
}

void pretty_printer::writeArrayNumbers(nbt_string_view name, std::size_t size) {
  auto multiplier = (size == 1 ? "" : "s");
  append(boost::format("[%1% %2%%3%]") % size % name % multiplier);
}

void pretty_printer::append(const boost::format& format) {
  append(format.str());
}

void pretty_printer::writeType(tag_type t) {
  append(getNBTTypeName(t) + std::string(" : "));
}

std::ostream& operator<<(std::ostream& stream, const pretty_printer& printer) {
  stream << printer.string;

  return stream;
}

} // namespace redi::nbt