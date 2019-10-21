#include "color_sets.h"

namespace data {

  const char color_sets::s_ranges[] = "ranges";

  color_sets::color_sets ()
    : super(ranges)
    , ranges(s_ranges)
  {
    ranges().resize(5);
  }

  color_sets::color_sets (const color_sets& rhs)
    : color_sets()
  {
    super::operator=(rhs);
  }

} // namespace data
