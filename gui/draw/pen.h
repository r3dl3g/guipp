/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: pen definition
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>
#include <gui/core/color.h>
#include <gui++-draw-export.h>


namespace gui {

  namespace draw {

    struct GUIPP_DRAW_EXPORT /*immutable*/ pen {

#if defined(GUIPP_WIN) || defined(GUIPP_X11) || defined(GUIPP_X11) || defined(GUIPP_QT)
      typedef float size_type;
#endif // GUIPP_WIN | GUIPP_X11

      enum struct Style : int {
        solid =       IF_WIN32_X11_QT_ELSE(PS_SOLID,       LineSolid,              Qt::PenStyle::SolidLine,       1),
        dash =        IF_WIN32_X11_QT_ELSE(PS_DASH,        LineOnOffDash,          Qt::PenStyle::DashLine,        2),
        dot =         IF_WIN32_X11_QT_ELSE(PS_DOT,         LineOnOffDash | 0x010,  Qt::PenStyle::DotLine,         3),
        dashDot =     IF_WIN32_X11_QT_ELSE(PS_DASHDOT,     LineOnOffDash | 0x020,  Qt::PenStyle::DashDotLine,     4),
        dashDotDot =  IF_WIN32_X11_QT_ELSE(PS_DASHDOTDOT,  LineOnOffDash | 0x030,  Qt::PenStyle::DashDotDotLine,  5),
        hairLine =    IF_WIN32_X11_QT_ELSE(PS_NULL,        LineSolid | 0x040,      Qt::PenStyle::SolidLine,       6),
        insideFrame = IF_WIN32_X11_QT_ELSE(PS_INSIDEFRAME, LineSolid | 0x050,      Qt::PenStyle::SolidLine,       7),
        invisible =   IF_WIN32_X11_QT_ELSE(PS_NULL,        0x0,                    Qt::PenStyle::NoPen,           0)
      };

      pen (const os::color& = color::black, size_type = 1, Style = Style::solid);
      pen (const pen&);
      ~pen ();

#ifdef GUIPP_WIN
      pen (os::pen);

      inline operator os::pen () const {
        return id;
      }

#endif // GUIPP_WIN

      os::color color () const;
      size_type size () const;
      os::size_type os_size () const;
      Style style () const;

      pen with_size (size_type) const;
      pen with_os_size (os::size_type) const;
      pen with_style (Style) const;
      pen with_color (const os::color&) const;

      bool operator== (const pen&) const;

    private:
#ifdef GUIPP_WIN
      os::pen id;
      os::win32::pen_type info;
#endif // GUIPP_WIN
#if defined(GUIPP_X11) || defined(GUIPP_QT)
      const os::color m_color;
      const size_type m_size;
      const Style m_style;
#endif // GUIPP_X11 || GUIPP_QT

    };

    const extern pen default_pen;

  }

}
