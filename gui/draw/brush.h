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
 * @brief     C++ API: brush definition
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

    struct GUIPP_DRAW_EXPORT /*immutable*/ brush {

      enum Style {
        solid = IF_WIN32_ELSE(BS_SOLID, FillSolid)
      };

      brush (const os::color& = color::black, Style = solid);
      brush (const brush&);
      ~brush ();

#ifdef WIN32
      brush (os::brush);

      operator os::brush () const;
#endif // WIN32

      os::color color () const;
      Style style () const;

      brush with_style(Style) const;
      brush with_color (const os::color&) const;

      bool operator== (const brush&) const;

    private:
#ifdef WIN32
      void destroy ();

      mutable os::brush id;
      os::win32::brush_type info;
#endif // WIN32
#ifdef X11
      const os::color m_color;
      const Style m_style;
#endif // X11

    };

    const extern brush default_brush;

  }

}
