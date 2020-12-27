/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
        solid = IF_WIN32_X11_QT_ELSE(BS_SOLID, FillSolid, Qt::BrushStyle::SolidPattern, 0),
        invisible = IF_WIN32_X11_QT_ELSE(BS_NULL, 0, Qt::BrushStyle::NoBrush, 0)
      };

      brush (const os::color& = color::black, Style = solid);
      brush (const brush&);
      ~brush ();

#ifdef GUIPP_WIN
      brush (os::brush);

      operator os::brush () const;
#endif // GUIPP_WIN

      os::color color () const;
      Style style () const;

      brush with_style(Style) const;
      brush with_color (const os::color&) const;

      bool operator== (const brush&) const;

    private:
#ifdef GUIPP_WIN
      void destroy ();

      mutable os::brush id;
      os::win32::brush_type info;
#endif // GUIPP_WIN
#if defined(GUIPP_X11) || defined(GUIPP_QT)
      const os::color m_color;
      const Style m_style;
#endif // GUIPP_X11 || GUIPP_QT

    };

    const extern brush default_brush;

  }

}
