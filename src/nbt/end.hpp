#pragma once

#include "basic.hpp"

namespace redi::nbt {

class tag_end : public basic_tag<tag_end> {
public:
  static constexpr tag_type type = tag_type::type_end;
};

}