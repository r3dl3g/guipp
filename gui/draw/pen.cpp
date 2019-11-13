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
#include <gui/draw/pen.h>


namespace gui {

  namespace draw {

#ifdef WIN32
    const pen default_pen((os::pen)GetStockObject(BLACK_PEN));

    pen::pen (os::pen id)
      : id(id)
    {
      GetObject(id, sizeof (os::win32::pen_type), &info);
    }

    pen::pen (const os::color& color, size_type width, Style style)
      : id(nullptr)
    {
      info.elpColor = color;
      info.elpPenStyle = static_cast<DWORD>(style);
      auto w = core::global::scale<os::size_type, size_type>(width);
      info.elpWidth = w;
      info.elpBrushStyle = BS_SOLID;
      info.elpHatch = 0;
      info.elpNumEntries = 0;
      info.elpStyleEntry[0] = 0;
      LOGBRUSH LogBrush;
      LogBrush.lbColor = color;
      LogBrush.lbStyle = BS_SOLID;
      LogBrush.lbHatch = 0;
      id = ExtCreatePen(info.elpPenStyle | PS_GEOMETRIC | PS_ENDCAP_SQUARE | PS_JOIN_MITER, w, &LogBrush, 0, NULL);
    }

    pen::pen (const pen& rhs)
      : pen(rhs.info.elpColor, (size_type)rhs.info.elpWidth, (Style)rhs.info.elpPenStyle)
    {}

    pen::~pen () {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
    }

    os::color pen::color () const {
      return info.elpColor;
    }

    pen::size_type pen::size () const {
      return core::global::scale<size_type, os::size_type>(info.elpWidth);
    }

    os::size_type pen::os_size () const {
      return info.elpWidth;
    }

    pen::Style pen::style () const {
      return (pen::Style)info.elpPenStyle;
    }

    bool pen::operator== (const pen& rhs) const {
      return ((info.elpColor == rhs.info.elpColor) &&
              (info.elpPenStyle == rhs.info.elpPenStyle) &&
              (info.elpWidth == rhs.info.elpWidth));
    }

#endif // WIN32
    pen pen::with_size (size_type sz) const {
      return pen(color(), sz, style());
    }

    pen pen::with_os_size(os::size_type sz) const {
      return pen(color(), core::global::scale<size_type, os::size_type>(sz), style());
    }

    pen pen::with_style (Style s) const {
      return pen(color(), size(), s);
    }

    pen pen::with_color (const os::color& c) const {
      return pen(c, size(), style());
    }

#ifdef X11
    const pen default_pen;

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

    pen::~pen () {}

    os::color pen::color () const {
      return m_color;
    }

    pen::size_type pen::size () const {
      return m_size;
    }

    os::size_type pen::os_size () const {
      return core::global::scale<os::size_type, size_type>(m_size);
    }

    pen::Style pen::style () const {
      return m_style;
    }

    bool pen::operator== (const pen& rhs) const {
      return ((m_color == rhs.m_color) &&
              (m_style == rhs.m_style) &&
              (m_size == rhs.m_size));
    }

#endif // X11

  }

}
