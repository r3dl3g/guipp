/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <cstdint>
#include <ostream>
#include <array>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui++-core-export.h>

#ifdef GUIPP_WIN
# define IF_WIN32_ELSE(A, B) A
# define IF_X11_ELSE(A, B) B
# define IF_QT_ELSE(A, B) B
# define IF_COCOA_ELSE(A, B) B
# define IF_WIN32_X11_QT_ELSE(A, B, C, D) A
#elif defined(GUIPP_X11)
# define IF_WIN32_ELSE(A, B) B
# define IF_X11_ELSE(A, B) A
# define IF_QT_ELSE(A, B) B
# define IF_COCOA_ELSE(A, B) B
# define IF_WIN32_X11_QT_ELSE(A, B, C, D) B
#elif defined(GUIPP_QT)
# define IF_WIN32_ELSE(A, B) B
# define IF_X11_ELSE(A, B) B
# define IF_QT_ELSE(A, B) A
# define IF_COCOA_ELSE(A, B) B
# define IF_WIN32_X11_QT_ELSE(A, B, C, D) C
#elif defined(GUIPP_COCOA)
# define IF_WIN32_ELSE(A, B) B
# define IF_X11_ELSE(A, B) B
# define IF_QT_ELSE(A, B) B
# define IF_COCOA_ELSE(A, B) A
# define IF_WIN32_X11_QT_ELSE(A, B, C, D) D
#else
# pragma error "Unknown target system"
#endif

#if __clang__
# define IF_CLANG_ELSE(A, B) A
# define IF_GCC_ELSE(A, B) B
# define IF_MINGW_ELSE(A, B) B
# define IF_MSC_ELSE(A, B) B
#elif __GNUC__
# define IF_CLANG_ELSE(A, B) B
# define IF_GCC_ELSE(A, B) A
# define IF_MINGW_ELSE(A, B) B
# define IF_MSC_ELSE(A, B) B
#elif __MINGW32__ || __MINGW64__
# define IF_CLANG_ELSE(A, B) B
# define IF_GCC_ELSE(A, B) B
# define IF_MINGW_ELSE(A, B) A
# define IF_MSC_ELSE(A, B) B
#elif _MSC_VER
# define IF_CLANG_ELSE(A, B) B
# define IF_GCC_ELSE(A, B) B
# define IF_MINGW_ELSE(A, B) B
# define IF_MSC_ELSE(A, B) A
#else
# pragma error "Unknown compiler"
#endif

#if defined(GUIPP_BUILD_FOR_MOBILE)
# define IF_MOBILE_ELSE(A, B) A
#else
# define IF_MOBILE_ELSE(A, B) B
#endif

namespace gui {

  namespace core {

    typedef unsigned char byte;
    typedef byte* byteptr;
    typedef const byte* cbyteptr;

    typedef unsigned short word;
    typedef word* wordptr;
    typedef const word* cwordptr;

    // --------------------------------------------------------------------------
    enum class bit_order_t : bool {
      lsb_first,
      msb_first
    };

    // --------------------------------------------------------------------------
    enum class byte_order_t : bool {
      little_endian,
      big_endian
    };

    namespace os {

      // --------------------------------------------------------------------------
      namespace detail {
        constexpr union {
          uint32_t i;
          char c[4];
        } endianess_check = { 0x01020300 };
      }

      // --------------------------------------------------------------------------
      enum class platform_t : byte {
        win32,
        x11,
        cocoa,
        qt
      };

      const platform_t system_platform = IF_WIN32_X11_QT_ELSE(platform_t::win32,
                                                              platform_t::x11,
                                                              platform_t::qt,
                                                              platform_t::cocoa);

      const bit_order_t bitmap_bit_order = IF_WIN32_X11_QT_ELSE(bit_order_t::msb_first,
                                                                bit_order_t::lsb_first,
                                                                bit_order_t::lsb_first,
                                                                bit_order_t::msb_first);

      const bool bitmap_bit_white = IF_WIN32_X11_QT_ELSE(false, true, true, false);

      const byte_order_t bitmap_byte_order =
#if defined(__i386__) || defined(__x86_64__) || defined(__arm__)
                                              byte_order_t::little_endian;
#elif defined(__powerpc64__)
                                              byte_order_t::big_endian;
#else
                                              byte_order_t(detail::endianess_check.c[0] == 1);
#endif
      // --------------------------------------------------------------------------
      enum class ui_t : byte {
        desktop,
        mobile
      };

      const ui_t system_ui = IF_MOBILE_ELSE(ui_t::mobile, ui_t::desktop);

      // --------------------------------------------------------------------------

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
    template<bool W>
    struct bw_bits {};

    template<>
    struct GUIPP_CORE_EXPORT bw_bits<false> {
      static byte value[2];
      static constexpr bool white = false;
      static constexpr bool black = true;
    };

    template<>
    struct GUIPP_CORE_EXPORT bw_bits<true> {
      static byte value[2];
      static constexpr bool white = true;
      static constexpr bool black = false;
    };

    // --------------------------------------------------------------------------
    template<byte bit, bit_order_t O = os::bitmap_bit_order>
    struct system_bit_mask {};

    template<byte bit>
    struct system_bit_mask<bit, bit_order_t::lsb_first> : lsb_bit_mask<bit> {};

    template<byte bit>
    struct system_bit_mask<bit, bit_order_t::msb_first> : msb_bit_mask<bit> {};

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT system_bw_bits : public bw_bits<os::bitmap_bit_white> {
      typedef bw_bits<os::bitmap_bit_white> super;
      static byte mask[8];
      static byte shift[8];
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

      inline const bit_wrapper& operator= (T b) {
#ifdef GUIPP_WIN
        set_bit(value, bit, static_cast<byte>(b) != 0);
#else
        set_bit(value, bit, static_cast<bool>(b));
#endif
        return *this;
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
    constexpr uint32_t reverse_byte_order (uint32_t n) {
      return ((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000) >> 8) | ((n & 0xff000000) >> 24);
    }

    constexpr uint16_t reverse_byte_order (uint16_t n) {
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
    inline T round_up (U v) {
      return static_cast<T>(std::ceil(v));
    }

    template<typename T, typename U>
    inline T round_down (U v) {
      return static_cast<T>(std::floor(v));
    }

  } // namespace core

} // namespace gui

namespace std {

  GUIPP_CORE_EXPORT ostream& print_enum (ostream& out, int t, const char* names[], std::size_t n);

  template<typename T, std::size_t N>
  inline ostream& print_enum (ostream& out, T t, const char* (&names)[N]) {
    return print_enum(out, static_cast<int>(t), names, N);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::core::bit_order_t);

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::core::byte_order_t);

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::core::os::platform_t);

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::core::os::ui_t);

} // namespace std
