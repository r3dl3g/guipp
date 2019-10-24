
#pragma once

#include <persistent/ptree_persistent.h>

namespace data {

  // --------------------------------------------------------------------------
  struct range : public persistent::ptree_struct<persistent::byte, persistent::byte> {
    typedef persistent::ptree_struct<persistent::byte, persistent::byte> super;
    typedef persistent::byte::value_type byte;

    static const char s_min[];
    static const char s_max[];

    range (byte min = 0, byte max = 255);
    range (const range&);

    void extend (byte v);

    persistent::byte min;
    persistent::byte max;
  };

} // namespace data