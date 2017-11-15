/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: basic typedefs
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <cmath>


namespace gui {

  namespace os {

    // --------------------------------------------------------------------------
    enum class platform : int {
      win32,
      x11
    };

    // --------------------------------------------------------------------------
  } // namespace os

  typedef uint8_t byte;
  typedef byte* byteptr;
  typedef const byte* cbyteptr;

  typedef uint16_t word;
  typedef word* wordptr;
  typedef const word* cwordptr;

  // --------------------------------------------------------------------------
  enum class bit_order : bool {
    lsb,
    msb
  };

  // --------------------------------------------------------------------------
  template<typename T, T mask>
  struct bit_mask {
    static bool is_set (T value) {
      return (value & mask) == mask;
    }

    static bool is_not_set (T value) {
      return (value & mask) == 0;
    }

  };

  // --------------------------------------------------------------------------
  template<byte bit>
  struct msb_bit_mask {
    static constexpr byte shift = 7 - bit;
    static constexpr byte value = 0x01 << shift;
  };

  template<byte bit>
  struct lsb_bit_mask {
    static constexpr byte shift = bit;
    static constexpr byte value = 0x01 << shift;
  };

  // --------------------------------------------------------------------------
  template<bit_order O>
  struct bw_bits {};

  template<>
  struct bw_bits<bit_order::lsb> {
    static constexpr byte value[2] = {0, 0xff};
    static constexpr bool white = true;
    static constexpr bool black = false;
  };

  template<>
  struct bw_bits<bit_order::msb> {
    static constexpr byte value[2] = {0xff, 0};
    static constexpr bool white = false;
    static constexpr bool black = true;
  };

  // --------------------------------------------------------------------------
  constexpr byte reverse_lookup_table[16] = {
    0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
    0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
  };

  constexpr byte reverse_bit_order (byte n) {
    // Reverse the top and bottom nibble then swap them.
    return (reverse_lookup_table[n & 0b1111] << 4) | reverse_lookup_table[n >> 4];
  }

  // --------------------------------------------------------------------------
  // the honour goes to user79758 : http://stackoverflow.com/a/4609795
  template<typename T>
  constexpr int signum (T x, std::false_type is_signed) {
    return T(0) < x;
  }

  template<typename T>
  constexpr int signum (T x, std::true_type is_signed) {
    return (T(0) < x) - (x < T(0));
  }

  template<typename T>
  constexpr int signum (T x) {
    return signum(x, std::is_signed<T>());
  }

  template<typename T>
  constexpr T div_ceil (T num, T div) {
    return (num - 1 + div) / div;
  }

  template<typename T, typename U>
  inline T roundup (U v) {
    return static_cast<T>(std::ceil(v));
  }

  template<typename T, typename U>
  inline T rounddown (U v) {
    return static_cast<T>(std::floor(v));
  }

  // --------------------------------------------------------------------------
} // namespace gui
