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

#ifdef GUIPP_QT
#include <QtGui/QPainter>
#endif // GUIPP_QT


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

#ifdef GUIPP_X11
    using namespace core::global;

    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen& p) {
      gui::os::instance display = get_instance();
      XSetForeground(display, g, p.color());
      const auto line_width = p.os_size();
      const int	line_style = static_cast<int>(p.style()) & 0x0F;
      const int	cap_style = static_cast<int>(p.cap());
      const int	join_style = static_cast<int>(p.join());

      XSetLineAttributes(display, g, line_width, line_style, cap_style, join_style);

      if (static_cast<int>(p.style()) & 0x0F0) {
        const char s = core::global::scale_to_native<int, float>(1);
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

#ifndef GUIPP_USE_XFT
    template<>
    void Use<font>::set(const font& f) {
      XSetFont(core::global::get_instance(), g, f);
    }
#endif // GUIPP_USE_XFT

#endif // GUIPP_X11

#ifdef GUIPP_QT
    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen& p) {
      g->setPen(QPen(QBrush(p.color()), p.os_size(),
                     static_cast<Qt::PenStyle>(p.style()),
                     static_cast<Qt::PenCapStyle>(p.cap()),
                     static_cast<Qt::PenJoinStyle>(p.join())));
    }

    template<>
    void Use<brush>::set (const brush& b) {
      g->setBrush(QBrush(b.color(), static_cast<Qt::BrushStyle>(b.style())));
    }

    template<>
    void Use<font>::set (const font& f) {
      g->setFont(QFont(QString::fromStdString(f.name()), f.size(), f.thickness(), f.italic()));
    }

#endif // GUIPP_QT

  } // namespace draw

} // namespace gui
