
#pragma once

#include "range.h"

namespace data {

  struct hsv_range : public persistent::ptree_struct<persistent::type<range>,
                                                     persistent::type<range>,
                                                     persistent::type<range>,
                                                     persistent::string> {
    typedef persistent::ptree_struct<persistent::type<range>,
                                     persistent::type<range>,
                                     persistent::type<range>,
                                     persistent::string> super;

    static const char s_hue[];
    static const char s_saturation[];
    static const char s_value[];
    static const char s_name[];

    hsv_range (const range& h = range(0, 180), const range& s = range(), const range& v = range(), const std::string& n = std::string());
    hsv_range (const hsv_range&);

    persistent::type<range> hue;
    persistent::type<range> saturation;
    persistent::type<range> value;
    persistent::string name;
  };

} // namespace data
