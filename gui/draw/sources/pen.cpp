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
#include "pen.h"


namespace gui {

  namespace draw {

#ifdef WIN32
    const pen pen::default_pen((os::pen)GetStockObject(BLACK_PEN));

    pen::pen(os::pen id)
      :id(id)
    {
      GetObject(id, sizeof(core::pen_type), &info);
    }

    pen::pen(const draw::color& color, Style style, size_type width)
      : id(CreatePen(style, width, color))
    {
      GetObject(id, sizeof(core::pen_type), &info);
    }

    pen::pen(const pen& rhs)
      : id(CreatePenIndirect(&rhs.info))
      , info(rhs.info)
    {}

    pen::~pen() {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
    }

    draw::color pen::color() const {
      return info.lopnColor;
    }

    pen::size_type pen::size() const {
      return info.lopnWidth.x;
    }

    pen::Style pen::style() const {
      return (pen::Style)info.lopnStyle;
    }

    pen pen::with_size(size_type sz) const {
      core::pen_type newType = info;
      newType.lopnWidth = { sz, sz };
      return pen(CreatePenIndirect(&newType));
    }

    pen pen::with_style(Style s) const {
      core::pen_type newType = info;
      newType.lopnStyle = s;
      return pen(CreatePenIndirect(&newType));
    }

    pen pen::with_color(const draw::color& c) const {
      core::pen_type newType = info;
      newType.lopnColor = c;
      return pen(CreatePenIndirect(&newType));
    }

    bool pen::operator== (const pen& rhs) const {
      return ((info.lopnColor == rhs.info.lopnColor) &&
              (info.lopnStyle == rhs.info.lopnStyle) &&
              (info.lopnWidth.x == rhs.info.lopnWidth.x) &&
              (info.lopnWidth.y == rhs.info.lopnWidth.y));
    }

#endif // WIN32
#ifdef X11
    const pen pen::default_pen;

    pen::pen(const draw::color& color, Style style, size_type size)
      : m_color(color)
      , m_style(style)
      , m_size(size)
    {}

    pen::pen(const pen& rhs)
      : m_color(rhs.m_color)
      , m_style(rhs.m_style)
      , m_size(rhs.m_size)
    {}

    pen::~pen() {
    }

    draw::color pen::color() const {
      return m_color;
    }

    pen::size_type pen::size() const {
      return m_size;
    }

    pen::Style pen::style() const {
      return m_style;
    }

    pen pen::with_size(size_type sz) const {
      return pen(m_color, m_style, sz);
    }

    pen pen::with_style(Style s) const {
      return pen(m_color, s, m_size);
    }

    pen pen::with_color(const draw::color& c) const {
      return pen(c, m_style, m_size);
    }

    bool pen::operator== (const pen& rhs) const {
      return ((m_color == rhs.m_color) &&
              (m_style == rhs.m_style) &&
              (m_size == rhs.m_size));
    }

#endif // X11

  }

}
