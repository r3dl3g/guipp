
#pragma once

#include "color_sets.h"

namespace data {

  struct redimage_settings : public persistent::ptree_struct<persistent::type<color_sets>,
                                                             persistent::string,
                                                             persistent::integer> {
    typedef persistent::ptree_struct<persistent::type<color_sets>,
                                     persistent::string,
                                     persistent::integer> super;

    static const char s_color_sets[];
    static const char s_last_path[];
    static const char s_version[];

    redimage_settings ();
    redimage_settings (const redimage_settings&);

    persistent::type<color_sets> colors;
    persistent::string last_path;
    persistent::integer version;
  };

} // namespace data
