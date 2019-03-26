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

#ifdef WIN32
      typedef int size_type;
#endif // WIN32
#ifdef X11
      typedef unsigned int size_type;
#endif // X11

      enum struct Style : int {
        solid = IF_WIN32_ELSE(PS_SOLID, LineSolid),
        dash = IF_WIN32_ELSE(PS_DASH, LineOnOffDash),
        dot = IF_WIN32_ELSE(PS_DOT, LineOnOffDash | 0x010),
        dashDot = IF_WIN32_ELSE(PS_DASHDOT, LineOnOffDash | 0x020),
        dashDotDot = IF_WIN32_ELSE(PS_DASHDOTDOT, LineOnOffDash | 0x030),
        hairLine = IF_WIN32_ELSE(PS_NULL, LineSolid | 0x040),
        insideFrame = IF_WIN32_ELSE(PS_INSIDEFRAME, LineSolid | 0x050)
      };

      pen (const os::color& = color::black, size_type = 1, Style = Style::solid);
      pen (const pen&);
      ~pen ();

#ifdef WIN32
      pen (os::pen);

      inline operator os::pen () const {
        return id;
      }

#endif // WIN32

      os::color color () const;
      size_type size () const;
      Style style () const;

      pen with_size(size_type) const;
      pen with_style(Style) const;
      pen with_color (const os::color&) const;

      bool operator== (const pen&) const;

    private:

#ifdef WIN32
      os::pen id;
      os::win32::pen_type info;
#endif // WIN32
#ifdef X11
      const os::color m_color;
      const size_type m_size;
      const Style m_style;
#endif // X11

    };

    const extern pen default_pen;

  }

}
