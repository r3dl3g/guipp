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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"
#include "color.h"


namespace gui {

  namespace draw {

    struct /*immutable*/ pen {

      const static pen default_pen;

#ifdef WIN32
      typedef int size_type;
#endif // WIN32
#ifdef X11
      typedef unsigned int size_type;
#endif // X11

      enum struct Style : int {
#ifdef WIN32
        solid = PS_SOLID,
        dash = PS_DASH,
        dot = PS_DOT,
        dashDot = PS_DASHDOT,
        dashDotDot = PS_DASHDOTDOT,
        hairLine = PS_NULL,
        insideFrame = PS_INSIDEFRAME
#elif X11
        solid = LineSolid,
        dash = LineOnOffDash,
        dot = LineOnOffDash | 0x010,
        dashDot = LineOnOffDash | 0x020,
        dashDotDot = LineOnOffDash | 0x030,
        hairLine = LineSolid | 0x040,
        insideFrame = LineSolid | 0x050
#else

#pragma error "Unknown target system"

#endif // WIN32
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

      pen with_size (size_type) const;
      pen with_style (Style) const;
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

  }

}
