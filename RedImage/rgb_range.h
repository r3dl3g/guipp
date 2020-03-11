
#pragma once

#include "range.h"

namespace data {

  struct rgb_range : public persistent::ptree_struct<range, range, range> {
    typedef persistent::ptree_struct<range, range, range> super;

    static const char s_red[];
    static const char s_green[];
    static const char s_blue[];

    rgb_range (const range& r = range(), const range& g = range(), const range& b = range());
    rgb_range (const rgb_range&);

    persistent::type<range> red;
    persistent::type<range> green;
    persistent::type<range> blue;
  };

} // namespace data
