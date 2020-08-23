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
#include <gui/draw/font.h>

namespace gui {

  namespace draw {

#ifdef X11
    using namespace core::global;

    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen& p) {
      gui::os::instance display = get_instance();
      XSetForeground(display, g, p.color());
      const auto sz = p.os_size();
      XSetLineAttributes(display, g, sz, static_cast<int>(p.style()) & 0x0F,
                         p.style() == pen::Style::solid ? ((sz % 2) == 0 ? CapProjecting : CapRound) : CapButt,
                         p.style() == pen::Style::solid ? JoinMiter : JoinBevel);
      if (static_cast<int>(p.style()) & 0x0F0) {
        const char s = core::global::scale<int, float>(1);
        const char l = s * 4;
        switch (p.style()) {
        case pen::Style::dot:
          static const char dots[] = {s, s};
          XSetDashes(display, g, 0, dots, 2);
          break;
        case pen::Style::dashDot:
          static const char dash_dots[] = {l, l, s, l};
          XSetDashes(display, g, 0, dash_dots, 4);
          break;
        case pen::Style::dashDotDot:
          static const char dash_dot_dots[] = {l, l, s, char(s * 2), s, l};
          XSetDashes(display, g, 0, dash_dot_dots, 6);
          break;
        }
      }
    }

    template<>
    void Use<brush>::set (const brush& b) {
      gui::os::instance display = get_instance();
      XSetForeground(display, g, b.color());
      XSetFillStyle(display, g, b.style());
    }

#ifndef USE_XFT
    template<>
    void Use<font>::set(const font& f) {
      XSetFont(core::global::get_instance(), g, f);
    }
#endif // USE_XFT

#endif // X11

  } // namespace draw

} // namespace gui
