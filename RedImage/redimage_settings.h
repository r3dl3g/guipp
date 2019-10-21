
#pragma once

#include "color_sets.h"

namespace data {

  struct redimage_settings : public persistent::ptree_struct<persistent::type<color_sets>,
                                                             persistent::string> {
    typedef persistent::ptree_struct<persistent::type<color_sets>,
                                     persistent::string> super;

    static const char s_color_sets[];
    static const char s_last_path[];

    redimage_settings ();
    redimage_settings (const redimage_settings&);

    persistent::type<color_sets> colors;
    persistent::string last_path;
  };

} // namespace data
