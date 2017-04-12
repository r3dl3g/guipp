/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
* @brief     C++ API: brush definition
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

    struct /*immutable*/ brush {

      const static brush default_brush;

      enum Style {
#ifdef WIN32
        solid = BS_SOLID
#endif // Win32
#ifdef X11
        solid = FillSolid
#else

#pragma error "Unknown target system"

#endif // WIN32
      };

      brush(const draw::color& = draw::color::black(), Style = solid);
      brush(const brush&);
      ~brush();

#ifdef WIN32
      brush(os::brush);

      operator os::brush() const;
#endif // WIN32

      draw::color color() const;
      Style style() const;

      brush with_style(Style) const;
      brush with_color(const draw::color&) const;

      bool operator== (const brush&) const;

    private:
#ifdef WIN32
      void destroy();

      mutable os::brush id;
      os::win32::brush_type info;
#endif // WIN32
#ifdef X11
      const draw::color m_color;
      const Style m_style;
#endif // X11

    };

  }

}
