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
 * @brief     C++ API: basic window types
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_X11
# include <X11/XKBlib.h>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace std {

  namespace {

    const char* bit_order_t_names[] = {
      "lsb_first",
      "msb_first"
    };

    const char* byte_order_t_names[] = {
      "little_endian",
      "big_endian"
    };

    const char* platform_t_names[] = {
      "win32",
      "x11",
      "cocoa",
      "qt"
    };

    const char* ui_t_names[] = {
      "desktop",
      "mobile"
    };

  }

  GUIPP_CORE_EXPORT ostream& print_enum (ostream& out, int idx, const char* names[], std::size_t n) {
    if ((idx >= 0) && (idx < n)) {
      out << names[idx];
    } else {
      out << "Unknown";
    }
    return out;
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::core::bit_order_t v) {
    return print_enum(out, v, bit_order_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::core::byte_order_t v) {
    return print_enum(out, v, byte_order_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::core::os::platform_t v) {
    return print_enum(out, v, platform_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, const gui::core::os::ui_t v) {
    return print_enum(out, v, ui_t_names);
  }

} // namespace std
