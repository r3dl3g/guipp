
#include "range.h"

#include <algorithm>

namespace data {

  const char range::s_min[] = "min";
  const char range::s_max[] = "max";

  range::range (byte min_, byte max_)
    : super(min, max)
    , min(s_min, min_)
    , max(s_max, max_)
  {}

  range::range (const range& rhs)
    : range()
  {
    super::operator=(rhs);
  }

  void range::extend (byte v) {
    min(std::min(min(), v));
    max(std::max(max(), v));
  }

} // namespace data
