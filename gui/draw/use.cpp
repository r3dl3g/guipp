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
 * @brief     C++ API: basic window types
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/use.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>

namespace gui {

  namespace draw {

#ifdef X11
    using namespace core::global;

    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen& p) {
      os::instance display = get_instance();
      XSetForeground(display, g, p.color());
      XSetLineAttributes(display, g, p.size(), static_cast<int>(p.style()) & 0x0F, CapButt, JoinMiter);
      if (static_cast<int>(p.style()) & 0x0F0) {
        switch (p.style()) {
        case pen::Style::dot:
          static const char dots[] = {1, 1};
          XSetDashes(display, g, 0, dots, 2);
          break;
        case pen::Style::dashDot:
          static const char dash_dots[] = {4, 4, 1, 4};
          XSetDashes(display, g, 0, dash_dots, 4);
          break;
        case pen::Style::dashDotDot:
          static const char dash_dot_dots[] = {4, 4, 1, 2, 1, 4};
          XSetDashes(display, g, 0, dash_dot_dots, 6);
          break;
        }
      }
    }

    template<>
    void Use<brush>::set (const brush& b) {
      os::instance display = get_instance();
      XSetForeground(display, g, b.color());
      XSetFillStyle(display, g, b.style());
    }

#endif // X11

  } // namespace draw

} // namespace gui
