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
  enum class BPP : byte {
    Undefined = 0,
    BW = 1,
    GRAY = 8,
    RGB = 24,
    RGBA = 32
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
      GUIPP_CORE_EXPORT os::instance get_instance ();

      GUIPP_CORE_EXPORT void sync ();

      GUIPP_CORE_EXPORT BPP get_device_bits_per_pixel ();
      GUIPP_CORE_EXPORT int get_device_depth ();

      GUIPP_CORE_EXPORT os::key_state get_key_state ();

    }

  } // core

} //gui

// --------------------------------------------------------------------------

