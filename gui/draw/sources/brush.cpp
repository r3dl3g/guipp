/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
// Common includes
//
#include <ostream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "brush.h"


namespace gui {

  namespace draw {

#ifdef WIN32
    const brush brush::default_brush((os::brush)GetStockObject(WHITE_BRUSH));

    brush::brush(os::brush id)
      :id(id) {
      GetObject(id, sizeof(os::win32::brush_type), &info);
    }

    brush::brush(const draw::color& color, Style style)
      : id(0)
    {
      info.lbColor = color;
      info.lbStyle = style;
      info.lbHatch = 0;
      id = CreateBrushIndirect(&info);
    }

    brush::brush(const brush& rhs)
      : id(CreateBrushIndirect(&rhs.info))
      , info(rhs.info)
    {}

    brush::~brush() {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
    }

    draw::color brush::color() const {
      return info.lbColor;
    }

    brush::Style brush::style() const {
      return (brush::Style)info.lbStyle;
    }

    brush brush::with_style(Style s) const {
      os::win32::brush_type newType = info;
      newType.lbStyle = s;
      return brush(CreateBrushIndirect(&newType));
    }

    brush brush::with_color(const draw::color& c) const {
      os::win32::brush_type newType = info;
      newType.lbColor = c;
      return brush(CreateBrushIndirect(&newType));
    }

    bool brush::operator== (const brush& rhs) const {
      return ((info.lbColor == rhs.info.lbColor) &&
              (info.lbStyle == rhs.info.lbStyle));
    }

#endif // WIN32
#ifdef X11
    const brush brush::default_brush;

    brush::brush(const draw::color& color, Style style)
      : m_color(color)
      , m_style(style)
    {}

    brush::brush(const brush& rhs)
      : m_color(rhs.m_color)
      , m_style(rhs.m_style)
    {}

    brush::~brush() {
    }

    draw::color brush::color() const {
      return m_color;
    }

    brush::Style brush::style() const {
      return m_style;
    }

    brush brush::with_style(Style s) const {
      return brush(m_color, s);
    }

    brush brush::with_color(const draw::color& c) const {
      return brush(c, m_style);
    }

    bool brush::operator== (const brush& rhs) const {
      return ((m_color == rhs.m_color) &&
              (m_style == rhs.m_style));
    }

#endif // X11

  }

}
