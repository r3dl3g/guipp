#include "hsv_range.h"

namespace data {

  const char hsv_range::s_hue[] = "hue";
  const char hsv_range::s_saturation[] = "saturation";
  const char hsv_range::s_value[] = "value";

  hsv_range::hsv_range (const range& h, const range& s, const range& v)
    : super(hue, saturation, value)
    , hue(s_hue, h)
    , saturation(s_saturation, s)
    , value(s_value, v)
  {}

  hsv_range::hsv_range (const hsv_range& rhs)
    : hsv_range()
  {
    super::operator=(rhs);
  }

} // namespace data
