#pragma once

#include "forward.hpp"

namespace redi::nbt {

class nbt_visitor {
public:
  virtual ~nbt_visitor() = 0;

  virtual void visit(tag_end&) {}
  virtual void visit(tag_byte&) {}
  virtual void visit(tag_short&) {}
  virtual void visit(tag_int&) {}
  virtual void visit(tag_long&) {}
  virtual void visit(tag_float&) {}
  virtual void visit(tag_double&) {}
  virtual void visit(tag_byte_array&) {}
  virtual void visit(tag_string&) {}
  virtual void visit(tag_list&) {}
  virtual void visit(tag_compound&) {}
  virtual void visit(root_tag&) {}
  virtual void visit(tag_int_array&) {}
  virtual void visit(tag_long_array&) {}
};

inline nbt_visitor::~nbt_visitor() = default;

class const_nbt_visitor {
public:
  virtual ~const_nbt_visitor() = 0;

  virtual void visit(const tag_end&) {}
  virtual void visit(const tag_byte&) {}
  virtual void visit(const tag_short&) {}
  virtual void visit(const tag_int&) {}
  virtual void visit(const tag_long&) {}
  virtual void visit(const tag_float&) {}
  virtual void visit(const tag_double&) {}
  virtual void visit(const tag_byte_array&) {}
  virtual void visit(const tag_string&) {}
  virtual void visit(const tag_list&) {}
  virtual void visit(const tag_compound&) {}
  virtual void visit(const root_tag&) {}
  virtual void visit(const tag_int_array&) {}
  virtual void visit(const tag_long_array&) {}
};

inline const_nbt_visitor::~const_nbt_visitor() = default;

}