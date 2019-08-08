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
#include <type_traits>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "base++-export.h"

#ifdef WIN32
# define IF_WIN32_ELSE(A, B) A
# define IF_X11_ELSE(A, B) B
# define IF_COCOA_ELSE(A, B) B
#elif defined(X11)
# define IF_WIN32_ELSE(A, B) B
# define IF_X11_ELSE(A, B) A
# define IF_COCOA_ELSE(A, B) B
#elif defined(COCOA)
# define IF_WIN32_ELSE(A, B) B
# define IF_X11_ELSE(A, B) B
# define IF_COCOA_ELSE(A, B) A
#else
# pragma error "Unknown target system"
#endif

namespace basepp {

  typedef unsigned char byte;
  typedef byte* byteptr;
  typedef const byte* cbyteptr;

  typedef unsigned short word;
  typedef word* wordptr;
  typedef const word* cwordptr;

  // --------------------------------------------------------------------------
  enum class bit_order : bool {
    lsb,
    msb
  };

  namespace os {

    // --------------------------------------------------------------------------
    enum class platform : byte {
      win32,
      x11,
      cocoa
    };

    const platform system_platform = IF_WIN32_ELSE(platform::win32, IF_X11_ELSE(platform::x11, platform::cocoa));
    const bit_order bitmap_bit_order = IF_WIN32_ELSE(bit_order::msb, bit_order::lsb);

  } // namespace os

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
  template<byte bit, basepp::bit_order O = os::bitmap_bit_order>
  struct system_bit_mask {};

  template<byte bit>
  struct system_bit_mask<bit, bit_order::lsb> : lsb_bit_mask<bit> {};

  template<byte bit>
  struct system_bit_mask<bit, bit_order::msb> : msb_bit_mask<bit> {};

  // --------------------------------------------------------------------------
  struct system_bw_bits : public bw_bits<os::bitmap_bit_order> {
    typedef bw_bits<os::bitmap_bit_order> super;
    static constexpr byte mask[8] = {
      system_bit_mask<0>::value,
      system_bit_mask<1>::value,
      system_bit_mask<2>::value,
      system_bit_mask<3>::value,
      system_bit_mask<4>::value,
      system_bit_mask<5>::value,
      system_bit_mask<6>::value,
      system_bit_mask<7>::value
    };
    static constexpr byte shift[8] = {
      system_bit_mask<0>::shift,
      system_bit_mask<1>::shift,
      system_bit_mask<2>::shift,
      system_bit_mask<3>::shift,
      system_bit_mask<4>::shift,
      system_bit_mask<5>::shift,
      system_bit_mask<6>::shift,
      system_bit_mask<7>::shift
    };
  };

  // --------------------------------------------------------------------------
  inline bool get_bit (byte value, byte bit) {
    const auto mask = system_bw_bits::mask[bit];
    return (value & mask) == mask;
  }

  inline void set_bit (byte& value, byte bit, bool b) {
    const auto mask = system_bw_bits::mask[bit];
    value = b ? (value | mask) : (value & ~mask);
  }

  // --------------------------------------------------------------------------
  template<typename T>
  struct bit_wrapper {
    inline bit_wrapper (byte& value, byte bit)
      : value(value)
      , bit(bit)
    {}

    inline operator T () const {
      return static_cast<T>(get_bit(value, bit));
    }

    inline T operator= (T b) {
#ifdef WIN32
      set_bit(value, bit, static_cast<byte>(b) != 0);
#else
      set_bit(value, bit, static_cast<bool>(b));
#endif
      return b;
    }

  private:
    byte& value;
    const byte bit;
  };

  // --------------------------------------------------------------------------
  template<typename T>
  struct bit_wrapper<T const> {
    inline bit_wrapper (byte value, byte bit)
      : value(value)
      , bit(bit)
    {}

    inline operator T () const {
      return static_cast<T>(get_bit(value, bit));
    }

  private:
    const byte value;
    const byte bit;
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
  constexpr u_int32_t reverse_byte_order (u_int32_t n) {
    return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24);
  }

  constexpr u_int16_t reverse_byte_order (u_int16_t n) {
    return ((n & 0xff) << 8) | ((n & 0xff00) >> 8);
  }

  // --------------------------------------------------------------------------
  // the honour goes to user79758 : http://stackoverflow.com/a/4609795
  template<typename T>
  constexpr int signum (T x, std::false_type) {
    return T(0) < x;
  }

  template<typename T>
  constexpr int signum (T x, std::true_type) {
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

} // namespace basepp
