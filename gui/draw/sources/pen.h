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
* @brief     C++ API: color definition
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

      const static pen default;

      typedef int size_type;

      enum Style {
#ifdef WIN32
        solid = PS_SOLID,
        dash = PS_DASH,
        dot = PS_DOT,
        dashDot = PS_DASHDOT,
        dashDotDot = PS_DASHDOTDOT,
        hairLine = PS_NULL,
        insideFrame = PS_INSIDEFRAME
#elif X11
        solid,
        dash,
        dot,
        dashDot,
        dashDotDot,
        hairLine,
        insideFrame
#else

#pragma error "Unknown target system"

#endif // WIN32
      };

      pen(const draw::color& = draw::color::black, Style = solid, size_type = 1);
      pen(const pen&);
      ~pen();

      inline operator core::pen_id() const {
        return id;
      }

      draw::color color() const;
      size_type size() const;
      Style style() const;

      pen with_size(size_type) const;
      pen with_style(Style) const;
      pen with_color(const draw::color&) const;

      bool operator== (const pen&) const;

      void swap(pen&);

    private:
      pen(core::pen_id);

      core::pen_id id;

#ifdef WIN32
      core::pen_type type;
#endif // WIN32

    };

  }

}
