#include "hsv_range.h"

namespace data {

  const char hsv_range::s_hue[] = "hue";
  const char hsv_range::s_saturation[] = "saturation";
  const char hsv_range::s_value[] = "value";
  const char hsv_range::s_name[] = "name";

  hsv_range::hsv_range (const range& h, const range& s, const range& v, const std::string& n)
    : super(hue, saturation, value, name)
    , hue(s_hue, h)
    , saturation(s_saturation, s)
    , value(s_value, v)
    , name(s_name, n)
  {}

  hsv_range::hsv_range (const hsv_range& rhs)
    : hsv_range()
  {
    super::operator=(rhs);
  }

} // namespace data
