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
#include <base/bits.h>
#include <base/logger.h>
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


#if defined(BASEPP_BUILT_AS_STATIC)
DECLARE_LOGGING_CORE(GUIPP_CORE_EXPORT)
#endif

namespace gui {

  namespace os {
    using namespace basepp::os;
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
  enum class PixelFormat : int {
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

  GUIPP_CORE_EXPORT std::ostream& operator<< (std::ostream&, PixelFormat);

  constexpr byte color_depths[] = {0, 1, 8, 24, 24, 24, 24, 24, 24};

  constexpr byte get_color_depth (PixelFormat px_fmt) {
    return color_depths[static_cast<byte>(px_fmt)];
  }

  constexpr byte bits_per_pixels[] = {0, 1, 8, 24, 24, 32, 32, 32, 32};

  constexpr byte get_bits_per_pixel (PixelFormat px_fmt) {
    return bits_per_pixels[static_cast<byte>(px_fmt)];
  }

  PixelFormat get_pixel_format (int pixel_format, basepp::byte_order byte_order);
  basepp::byte_order get_pixel_format_byte_order (PixelFormat px_fmt);

  template<PixelFormat px_fmt>
  struct color_depth {
    static constexpr int bits = color_depths[static_cast<byte>(px_fmt)];
  };

  template<PixelFormat px_fmt>
  struct bits_per_pixel {
    static constexpr int bits = bits_per_pixels[static_cast<byte>(px_fmt)];
  };

  // --------------------------------------------------------------------------
  enum class orientation : bool {
    vertical,
    horizontal
  };

  constexpr orientation operator! (orientation o) {
    return (o == orientation::vertical) ? orientation::horizontal : orientation::vertical;
  }

  // --------------------------------------------------------------------------
  enum class origin : byte {
    start,
    center,
    end
  };

  // --------------------------------------------------------------------------
  enum class alignment : byte {
    left,
    hcenter,
    right,
    top,
    vcenter,
    bottom
  };


  // --------------------------------------------------------------------------
  template<alignment>
  struct alignment_orientation {};

  template<>
  struct alignment_orientation<alignment::left> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<>
  struct alignment_orientation<alignment::hcenter> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<>
  struct alignment_orientation<alignment::right> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<>
  struct alignment_orientation<alignment::top> {
    static constexpr orientation value = orientation::vertical;
  };

  template<>
  struct alignment_orientation<alignment::vcenter> {
    static constexpr orientation value = orientation::vertical;
  };

  template<>
  struct alignment_orientation<alignment::bottom> {
    static constexpr orientation value = orientation::vertical;
  };

  // --------------------------------------------------------------------------
  template<alignment>
  struct alignment_origin {};

  template<>
  struct alignment_origin<alignment::left> {
    static constexpr origin value = origin::start;
  };

  template<>
  struct alignment_origin<alignment::hcenter> {
    static constexpr origin value = origin::center;
  };

  template<>
  struct alignment_origin<alignment::right> {
    static constexpr origin value = origin::end;
  };

  template<>
  struct alignment_origin<alignment::top> {
    static constexpr origin value = origin::start;
  };

  template<>
  struct alignment_origin<alignment::vcenter> {
    static constexpr origin value = origin::center;
  };

  template<>
  struct alignment_origin<alignment::bottom> {
    static constexpr origin value = origin::end;
  };

  // --------------------------------------------------------------------------
  namespace core {

    namespace global {

      GUIPP_CORE_EXPORT void init (os::instance instance);
      GUIPP_CORE_EXPORT void fini ();

      GUIPP_CORE_EXPORT void sync ();

      GUIPP_CORE_EXPORT os::instance get_instance ();
      GUIPP_CORE_EXPORT PixelFormat get_device_pixel_format ();
      GUIPP_CORE_EXPORT int get_device_depth ();
      GUIPP_CORE_EXPORT os::key_state get_key_state ();
      GUIPP_CORE_EXPORT double get_scale_factor ();
      GUIPP_CORE_EXPORT void set_scale_factor (double);

      namespace detail {

        template<bool, bool, typename _Tp = void>
        struct enable_if_both
        {};

          // Partial specialization for true, true.
        template<typename _Tp>
        struct enable_if_both<true, true, _Tp> {
          typedef _Tp type;
        };

      }

      /*
       * Scale matrix:
       * Output (R) | Input (T t) | Method
       * ------------------------------------------------------------
       * R = T      | T          | (R)t
       * X          | float      | (R)t
       * intX_t     | uintX_t    | (R)t
       * float      | uintX_t    | (R)max(0, t * get_scale_factor())
       * float      | intX_t     | (R)(t * get_scale_factor())
       * uintX_t    | intX_t     | (R)max(0, t)
       * ------------------------------------------------------------
       *
       * uncale matrix:
       * Input (T t) | Output (R) | Method
       * ------------------------------------------------------------
       * T           | R = T      | (R)t
       * X           | float      | (R)t
       * intX_t      | uintX_t    | (R)t
       * float       | uintX_t    | (R)max(0, t * get_scale_factor())
       * float       | intX_t     | (R)(t * get_scale_factor())
       * uintX_t     | intX_t     | (R)max(0, t)
       * ------------------------------------------------------------
       */

      template<typename R, typename T>
      struct scale {
        static R to (T v);
      };

      template<typename T>
      struct scale<T, T> {
        inline static T to (T v) {
          return v;
        }
      };

      template<typename R, typename T>
      struct scale<std::enable_if<std::is_floating_point<R>::value, R>::type,
                   std::enable_if<std::is_integral<T>::value, T>::type> {
        inline static R to (T v) {
          return static_cast<R>(round(v / get_scale_factor()));
        }
      };

      template<typename R, typename T>
      struct scale<std::enable_if<std::is_unsigned<R>::value, R>::type,
                   std::enable_if<std::is_floating_point<T>::value, T>::type> {
        inline static R to (T v) {
          return static_cast<R>(std::max<T>(0, round(v * get_scale_factor())));
        }
      };

      template<typename R, typename T>
      struct scale<std::enable_if<std::is_integral<R>::value, R>::type,
                   std::enable_if<std::is_floating_point<T>::value, T>::type> {
        inline static R to (T v) {
          return static_cast<R>(round(v * get_scale_factor()));
        }
      };

      template<typename R, typename T>
      struct scale<std::enable_if<std::is_unsigned<R>::value, R>::type,
                   std::enable_if<std::is_integral<T>::value, T>::type> {
        inline static R to (T v) {
          return static_cast<R>(std::max<T>(0, v));
        }
      };

      template<typename R, typename T>
      struct scale<std::enable_if<std::is_integral<R>::value, R>::type,
                   std::enable_if<std::is_integral<T>::value, T>::type> {
        inline static R to (T v) {
          return static_cast<R>(v);
        }
      };

      template<typename R, typename T>
      struct scale<std::enable_if<std::is_floating_point<R>::value, R>::type,
                   std::enable_if<std::is_floating_point<T>::value, T>::type> {
        inline static R to (T v) {
          return static_cast<R>(v);
        }
      };

    } // namespace global

  } // namespace core

} //namespace gui

// --------------------------------------------------------------------------

