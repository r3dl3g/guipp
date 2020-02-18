
#include "redimage_settings.h"

namespace data {

  const char redimage_settings::s_color_sets[] = "color_sets";
  const char redimage_settings::s_last_path[] = "last_path";
  const char redimage_settings::s_version[] = "version";
  const char redimage_settings::s_normalize[] = "normalize";

  redimage_settings::redimage_settings ()
    : super(colors, last_path, version, normalize)
    , colors(s_color_sets)
    , last_path(s_last_path)
    , version(s_version, 0)
    , normalize(s_normalize, true)
  {}

  redimage_settings::redimage_settings (const redimage_settings& rhs)
    : redimage_settings()
  {
    super::operator=(rhs);
  }

} // namespace data
