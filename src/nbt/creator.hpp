#pragma once

#include "tag.hpp"
#include "value.hpp"

namespace redi::nbt {

tag_unique_ptr create(tag_type t);
tag_value createValue(tag_type t);

} // namespace redi:nbt