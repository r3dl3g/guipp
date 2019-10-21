#include "rgb_range.h"

namespace data {

  const char rgb_range::s_red[] = "red";
  const char rgb_range::s_green[] = "green";
  const char rgb_range::s_blue[] = "blue";

  rgb_range::rgb_range (const range& r, const range& g, const range& b)
    : super(red, green, blue)
    , red(s_red, r)
    , green(s_green, g)
    , blue(s_blue, b)
  {}

  rgb_range::rgb_range (const rgb_range& rhs)
    : rgb_range()
  {
    super::operator=(rhs);
  }

} // namespace data
