#include "color_sets.h"

namespace data {

  const char color_sets::s_ranges[] = "ranges";
  const char color_sets::s_count[] = "count";

  color_sets::color_sets ()
    : super(count, ranges)
    , count(s_count, 0)
    , ranges(s_ranges)
  {}

  color_sets::color_sets (const color_sets& rhs)
    : color_sets()
  {
    super::operator=(rhs);
  }

  void color_sets::read (const persistent::io::ptree& pt) {
    ranges().clear();
    super::read(pt);
    if (count() < 2) {
      count(2);
    }
    ranges().resize(count());
    super::read(pt);
  }


} // namespace data
