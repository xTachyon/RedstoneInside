#include <limits>
#include <climits>

static_assert(CHAR_BIT == 8, "A byte has to have 8 bits");
static_assert(std::numeric_limits<float>::is_iec559, "Float must be iec559");
static_assert(std::numeric_limits<double>::is_iec559, "Double must be iec559");