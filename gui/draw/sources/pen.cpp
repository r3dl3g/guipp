/**
* @copyright (c) 2016-2017 Ing. Buero Rothfuss
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

    pen::pen (os::pen id)
      :id(id)
    {
      GetObject(id, sizeof(os::win32::pen_type), &info);
    }

    pen::pen (const os::color& color, size_type width, Style style)
      : id(CreatePen(static_cast<int>(style), width, color))
    {
      GetObject(id, sizeof(os::win32::pen_type), &info);
    }

    pen::pen (const pen& rhs)
      : id(CreatePenIndirect(&rhs.info))
      , info(rhs.info)
    {}

    pen::~pen () {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
    }

    os::color pen::color () const {
      return info.lopnColor;
    }

    pen::size_type pen::size () const {
      return info.lopnWidth.x;
    }

    pen::Style pen::style () const {
      return (pen::Style)info.lopnStyle;
    }

    pen pen::with_size (size_type sz) const {
      os::win32::pen_type newType = info;
      newType.lopnWidth = { sz, sz };
      return pen(CreatePenIndirect(&newType));
    }

    pen pen::with_style (Style s) const {
      os::win32::pen_type newType = info;
      newType.lopnStyle = static_cast<int>(s);
      return pen(CreatePenIndirect(&newType));
    }

    pen pen::with_color (const os::color& c) const {
      os::win32::pen_type newType = info;
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

    pen::pen (const os::color& color, size_type size, Style style)
      : m_color(color)
      , m_size(size)
      , m_style(style)
    {}

    pen::pen (const pen& rhs)
      : m_color(rhs.m_color)
      , m_size(rhs.m_size)
      , m_style(rhs.m_style)
    {}

    pen::~pen () {
    }

    os::color pen::color () const {
      return m_color;
    }

    pen::size_type pen::size () const {
      return m_size;
    }

    pen::Style pen::style () const {
      return m_style;
    }

    pen pen::with_size (size_type sz) const {
      return pen(m_color, sz, m_style);
    }

    pen pen::with_style (Style s) const {
      return pen(m_color, m_size, s);
    }

    pen pen::with_color (const os::color& c) const {
      return pen(c, m_size, m_style);
    }

    bool pen::operator== (const pen& rhs) const {
      return ((m_color == rhs.m_color) &&
              (m_style == rhs.m_style) &&
              (m_size == rhs.m_size));
    }

#endif // X11

  }

}
