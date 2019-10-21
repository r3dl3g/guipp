
#include "redimage_settings.h"

namespace data {

  const char redimage_settings::s_color_sets[] = "color_sets";
  const char redimage_settings::s_last_path[] = "last_path";

  redimage_settings::redimage_settings ()
    : super(colors, last_path)
    , colors(s_color_sets)
    , last_path(s_last_path)
  {}

  redimage_settings::redimage_settings (const redimage_settings& rhs)
    : redimage_settings()
  {
    super::operator=(rhs);
  }

} // namespace data
