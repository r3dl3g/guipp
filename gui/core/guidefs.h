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
#include <vector>
#include <string>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>
#include <logging/logger.h>
#include <gui++-core-export.h>


#ifdef WIN32
# include <gui/core/win32defs.h>
#endif // WIN32
#ifdef X11
# include <gui/core/x11defs.h>
#endif
#ifdef COCOA
# include <gui/core/cocoadefs.h>
#endif

#if !defined(WIN32) && !defined(X11) && !defined(COCOA)
# pragma error "Unknown target system"
#endif

#ifdef WIN32
#pragma warning (disable: 4251)
#endif


namespace gui {

  typedef uint8_t byte;
  typedef byte* byteptr;
  typedef const byte* cbyteptr;

  typedef uint16_t word;
  typedef word* wordptr;
  typedef const word* cwordptr;

  // --------------------------------------------------------------------------
  typedef std::vector<byte> blob;

  // --------------------------------------------------------------------------
  enum class pixel_format_t : int {
    Undefined = 0,
    BW,
    GRAY,
    RGB,
    BGR,
    RGBA,
    BGRA,
    ARGB,
    ABGR
  };

  GUIPP_CORE_EXPORT std::ostream& operator<< (std::ostream&, pixel_format_t);

  constexpr byte color_depths[] = {0, 1, 8, 24, 24, 24, 24, 24, 24};

  constexpr byte get_color_depth (pixel_format_t px_fmt) {
    return color_depths[static_cast<byte>(px_fmt)];
  }

  constexpr byte bits_per_pixels[] = {0, 1, 8, 24, 24, 32, 32, 32, 32};

  constexpr byte get_bits_per_pixel (pixel_format_t px_fmt) {
    return bits_per_pixels[static_cast<byte>(px_fmt)];
  }

  pixel_format_t get_pixel_format (int pixel_format, core::byte_order_t byte_order_t);
  core::byte_order_t get_pixel_format_byte_order (pixel_format_t px_fmt);

  template<pixel_format_t px_fmt>
  struct color_depth {
    static constexpr int bits = color_depths[static_cast<byte>(px_fmt)];
  };

  template<pixel_format_t px_fmt>
  struct bits_per_pixel {
    static constexpr int bits = bits_per_pixels[static_cast<byte>(px_fmt)];
  };

  // --------------------------------------------------------------------------
  enum class orientation_t : bool {
    vertical,
    horizontal
  };

  constexpr orientation_t operator! (orientation_t o) {
    return orientation_t(!static_cast<bool>(o));
  }

  constexpr orientation_t operator~ (orientation_t o) {
    return orientation_t(!static_cast<bool>(o));
  }

  constexpr orientation_t operator- (orientation_t lhs, orientation_t rhs) {
    return orientation_t(static_cast<byte>(lhs) - static_cast<byte>(rhs));
  }

  // --------------------------------------------------------------------------
  enum class origin_t : byte {
    start,
    center,
    end
  };

  // --------------------------------------------------------------------------
  enum class alignment_t : byte {
    left,
    hcenter,
    right,
    top,
    vcenter,
    bottom
  };


  // --------------------------------------------------------------------------
  template<alignment_t>
  struct alignment_orientation {};

  template<>
  struct alignment_orientation<alignment_t::left> {
    static constexpr orientation_t value = orientation_t::horizontal;
  };

  template<>
  struct alignment_orientation<alignment_t::hcenter> {
    static constexpr orientation_t value = orientation_t::horizontal;
  };

  template<>
  struct alignment_orientation<alignment_t::right> {
    static constexpr orientation_t value = orientation_t::horizontal;
  };

  template<>
  struct alignment_orientation<alignment_t::top> {
    static constexpr orientation_t value = orientation_t::vertical;
  };

  template<>
  struct alignment_orientation<alignment_t::vcenter> {
    static constexpr orientation_t value = orientation_t::vertical;
  };

  template<>
  struct alignment_orientation<alignment_t::bottom> {
    static constexpr orientation_t value = orientation_t::vertical;
  };

  // --------------------------------------------------------------------------
  template<alignment_t>
  struct alignment_origin {};

  template<>
  struct alignment_origin<alignment_t::left> {
    static constexpr origin_t value = origin_t::start;
  };

  template<>
  struct alignment_origin<alignment_t::hcenter> {
    static constexpr origin_t value = origin_t::center;
  };

  template<>
  struct alignment_origin<alignment_t::right> {
    static constexpr origin_t value = origin_t::end;
  };

  template<>
  struct alignment_origin<alignment_t::top> {
    static constexpr origin_t value = origin_t::start;
  };

  template<>
  struct alignment_origin<alignment_t::vcenter> {
    static constexpr origin_t value = origin_t::center;
  };

  template<>
  struct alignment_origin<alignment_t::bottom> {
    static constexpr origin_t value = origin_t::end;
  };

  // --------------------------------------------------------------------------
  template<origin_t, orientation_t>
  struct alignment {};

  template<>
  struct alignment <origin_t::start, orientation_t::horizontal> {
    static constexpr alignment_t value = alignment_t::left;
  };

  template<>
  struct alignment <origin_t::center, orientation_t::horizontal> {
    static constexpr alignment_t value = alignment_t::hcenter;
  };

  template<>
  struct alignment <origin_t::end, orientation_t::horizontal> {
    static constexpr alignment_t value = alignment_t::right;
  };

  template<>
  struct alignment <origin_t::start, orientation_t::vertical> {
    static constexpr alignment_t value = alignment_t::top;
  };

  template<>
  struct alignment <origin_t::center, orientation_t::vertical> {
    static constexpr alignment_t value = alignment_t::vcenter;
  };

  template<>
  struct alignment <origin_t::end, orientation_t::vertical> {
    static constexpr alignment_t value = alignment_t::bottom;
  };

  // --------------------------------------------------------------------------
  namespace core {

    namespace global {

      GUIPP_CORE_EXPORT void init (gui::os::instance instance);
      GUIPP_CORE_EXPORT void fini ();

      GUIPP_CORE_EXPORT void sync ();

      GUIPP_CORE_EXPORT gui::os::instance get_instance ();
      GUIPP_CORE_EXPORT pixel_format_t get_device_pixel_format ();
      GUIPP_CORE_EXPORT int get_device_depth ();
      GUIPP_CORE_EXPORT gui::os::key_state get_key_state ();
      GUIPP_CORE_EXPORT double get_scale_factor ();
      GUIPP_CORE_EXPORT void set_scale_factor (double);

      namespace detail {
        template<typename R, typename T>
        inline R scale_int_to_float (const T& v) {
          return static_cast<R>(round(static_cast<T>(v / get_scale_factor())));
        }

        template<typename R, typename T>
        inline R scale_float_to_sint (const T& v) {
          return static_cast<R>(round(static_cast<T>(v * get_scale_factor())));
        }

        template<typename R, typename T>
        inline R scale_float_to_uint (const T& v) {
          return static_cast<R>(std::max<T>(T(0), round(static_cast<T>(v * get_scale_factor()))));
        }

        template<typename R, typename T>
        inline R scale_int_to_uint (const T& v) {
          return static_cast<R>(std::max<T>(T(0), v));
        }

        template<typename R, typename T>
        inline R scale_forward (const T& v) {
          return static_cast<R>(v);
        }
      }

//      template<typename R, typename T>
//      R scale (const T& v);

      template<typename R, typename T>
      inline typename std::enable_if<std::is_floating_point<R>::value && std::is_integral<T>::value, R>::type scale (const T& v) {
        return detail::scale_int_to_float<R>(v);
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_unsigned<R>::value && std::is_floating_point<T>::value, R>::type scale (const T& v) {
        return detail::scale_float_to_uint<R>(v);
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_integral<R>::value && std::is_signed<R>::value && std::is_floating_point<T>::value, R>::type scale (const T& v) {
        return detail::scale_float_to_sint<R>(v);
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_unsigned<R>::value && std::is_integral<T>::value && std::is_signed<T>::value, R>::type scale (const T& v) {
        return detail::scale_int_to_uint<R>(v);
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_integral<R>::value && std::is_signed<R>::value && std::is_integral<T>::value, R>::type scale (const T& v) {
        return detail::scale_forward<R>(v);
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_floating_point<R>::value && std::is_floating_point<T>::value, R>::type scale (const T& v) {
        return detail::scale_forward<R>(v);
      }

    } // namespace global

  } // namespace core

} //namespace gui

// --------------------------------------------------------------------------

