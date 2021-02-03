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
#ifdef GUIPP_WIN
# include <gui/core/guidefs_win32.h>
#elif GUIPP_X11
# include <gui/core/guidefs_x11.h>
#elif GUIPP_QT
# include <gui/core/guidefs_qt.h>
#elif GUIPP_COCOA
# include <gui/core/guidefs_cocoa.h>
#else
#error Unknown target system
#endif // GUIPP_QT

#include <vector>
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>
#include <logging/logger.h>


#ifdef GUIPP_WIN
#pragma warning (disable: 4251)
#endif


namespace gui {

  namespace os {

    struct GUIPP_CORE_EXPORT surface {
      os::drawable id;
      os::graphics g;
    };

  }
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

  constexpr byte color_depths[] = {0, 1, 8, 24, 24, 24, 24, 24, 24};

  constexpr byte get_color_depth (pixel_format_t px_fmt) {
    return color_depths[static_cast<byte>(px_fmt)];
  }

  constexpr byte bits_per_pixels[] = {0, 1, 8, 24, 24, 32, 32, 32, 32};

  constexpr byte get_bits_per_pixel (pixel_format_t px_fmt) {
    return bits_per_pixels[static_cast<byte>(px_fmt)];
  }

  GUIPP_CORE_EXPORT pixel_format_t get_pixel_format (int pixel_format, core::byte_order_t byte_order_t);
  GUIPP_CORE_EXPORT core::byte_order_t get_pixel_format_byte_order (pixel_format_t px_fmt);

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
    return o == orientation_t::vertical ? orientation_t::horizontal : orientation_t::vertical;
  }

  constexpr orientation_t operator~ (orientation_t o) {
    return operator!(o);
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

      template<typename R, typename T>
      inline typename std::enable_if<std::is_signed<R>::value && !std::is_floating_point<R>::value, R>::type
      scale_from_native (const T& v) {
        return static_cast<R>(round(static_cast<T>(v / get_scale_factor())));
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_unsigned<R>::value, R>::type
      scale_from_native (const T& v) {
        return static_cast<R>(std::max<T>(T(0), static_cast<T>(round(v / get_scale_factor()))));
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_floating_point<R>::value, R>::type
      scale_from_native (const T& v) {
        return static_cast<R>(v / get_scale_factor());
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_signed<R>::value && !std::is_floating_point<R>::value, R>::type
      scale_to_native (const T& v) {
        return static_cast<R>(round(static_cast<T>(v * get_scale_factor())));
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_unsigned<R>::value, R>::type
      scale_to_native (const T& v) {
        return static_cast<R>(std::max<T>(T(0), round(static_cast<T>(v * get_scale_factor()))));
      }

      template<typename R, typename T>
      inline typename std::enable_if<std::is_floating_point<R>::value, R>::type
      scale_to_native (const T& v) {
        return static_cast<R>(v * get_scale_factor());
      }

    } // namespace global

  } // namespace core

} //namespace gui

namespace std {

  GUIPP_CORE_EXPORT ostream& operator<< (ostream&, gui::pixel_format_t);

  GUIPP_CORE_EXPORT ostream& operator<< (ostream&, gui::orientation_t);

  GUIPP_CORE_EXPORT ostream& operator<< (ostream&, gui::origin_t);

  GUIPP_CORE_EXPORT ostream& operator<< (ostream&, gui::alignment_t);

} // namespace std

// --------------------------------------------------------------------------

