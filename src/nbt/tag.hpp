#pragma once

#include <iostream>
#include <memory>
#include "type.hpp"
#include "forward.hpp"

namespace redi::nbt {

class tag {
public:
  virtual ~tag() = 0;

  bool is_number() const;
  bool is_scalar() const;
  bool is_vector() const;
  bool is_container() const;
  virtual std::size_t size() const { return 1; }

  virtual tag& assign(tag&& tag) = 0;
  virtual tag& assign(const tag& tag) = 0;

  virtual tag_type get_type() const { return tag_type::type_end; }

  virtual std::unique_ptr<tag> clone() const& = 0;
  virtual std::unique_ptr<tag> move() && = 0;

  virtual bool equals(const tag&) const { return false; }

  virtual void visit(nbt_visitor&) = 0;
  virtual void visit(const_nbt_visitor&) const = 0;
};

std::ostream& operator<<(std::ostream& stream, const tag& tag);

using tag_unique_ptr = std::unique_ptr<tag>;

} // namespace redi::nbt