
#pragma once

#include <persistent/ptree_persistent.h>

namespace data {

  // --------------------------------------------------------------------------
  struct range : public persistent::ptree_struct<uint8_t, uint8_t> {
    typedef persistent::ptree_struct<uint8_t, uint8_t> super;
    typedef uint8_t byte;

    static const char s_min[];
    static const char s_max[];

    range (byte min = 0, byte max = 255);
    range (const range&);

    void extend (byte v);

    persistent::uint8 min;
    persistent::uint8 max;
  };

} // namespace data
