
#pragma once

#include "hsv_range.h"

namespace data {

  struct color_sets : public persistent::ptree_struct<persistent::vector<hsv_range>> {
    typedef persistent::ptree_struct<persistent::vector<hsv_range>> super;

    static const char s_ranges[];

    color_sets ();
    color_sets (const color_sets&);

    persistent::vector<hsv_range> ranges;
  };

} // namespace data
