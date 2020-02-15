
#pragma once

#include "hsv_range.h"

namespace data {

  struct color_sets : public persistent::ptree_struct<persistent::integer, persistent::vector<hsv_range>> {
    typedef persistent::ptree_struct<persistent::integer, persistent::vector<hsv_range>> super;

    static const char s_ranges[];
    static const char s_count[];

    color_sets ();
    color_sets (const color_sets&);

    void read (const persistent::io::ptree& pt);

    persistent::integer count;
    persistent::vector<hsv_range> ranges;
  };

} // namespace data
