/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

#ifdef GUIPP_WIN
    const pen pen::invisible((os::win32::pen)GetStockObject(NULL_PEN));
    const pen pen::standard((os::win32::pen)GetStockObject(BLACK_PEN));

    pen::pen (os::win32::pen id)
      : id(id)
    {
      GetObject(id, sizeof (os::win32::pen_type), &info);
    }

    void pen::recreate (size_type width) {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
      info.elpWidth = core::global::scale_to_native<os::size_type, size_type>(width);
      LOGBRUSH LogBrush;
      LogBrush.lbColor = info.elpColor;
      LogBrush.lbStyle = BS_SOLID;
      LogBrush.lbHatch = 0;
      id = ExtCreatePen(info.elpPenStyle, info.elpWidth, &LogBrush, 0, NULL);
    }


    pen::pen (const os::color& color, size_type width, Style style, Cap cap, Join join)
      : id(nullptr)
    {
      info.elpColor = color;
      info.elpPenStyle = static_cast<DWORD>(style) | static_cast<DWORD>(cap) | static_cast<DWORD>(join) | PS_GEOMETRIC;
      info.elpBrushStyle = BS_SOLID;
      info.elpHatch = 0;
      info.elpNumEntries = 0;
      info.elpStyleEntry[0] = 0;
      recreate(width);
    }

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
      return core::global::scale_from_native<size_type, os::size_type>(info.elpWidth);
    }

    os::size_type pen::os_size () const {
      return info.elpWidth;
    }

    pen::Style pen::style () const {
      return (pen::Style)(info.elpPenStyle & PS_STYLE_MASK);
    }

    pen::Cap pen::cap () const {
      return (pen::Cap)(info.elpPenStyle & PS_ENDCAP_MASK);
    }

    pen::Join pen::join () const {
      return (pen::Join)(info.elpPenStyle & PS_JOIN_MASK);
    }

    bool pen::operator== (const pen& rhs) const {
      return ((info.elpColor == rhs.info.elpColor) &&
              (info.elpPenStyle == rhs.info.elpPenStyle) &&
              (info.elpWidth == rhs.info.elpWidth));
    }

#endif // GUIPP_WIN

    pen::pen (const pen& rhs)
      : pen(rhs.color(), rhs.size(), rhs.style(), rhs.cap(), rhs.join())
    {}

    pen pen::with_size (size_type sz) const {
      return pen(color(), sz, style(), cap(), join());
    }

    pen pen::with_os_size(os::size_type sz) const {
      return pen(color(), core::global::scale_from_native<size_type, os::size_type>(sz), style(), cap(), join());
    }

    pen pen::with_style (Style s) const {
      return pen(color(), size(), s, cap(), join());
    }

    pen pen::with_color (const os::color& c) const {
      return pen(c, size(), style(), cap(), join());
    }

    pen pen::with_cap (Cap c) const {
      return pen(color(), size(), style(), c, join());
    }

    pen pen::with_join (Join j) const {
      return pen(color(), size(), style(), cap(), j);
    }

#if defined(GUIPP_X11) || defined(GUIPP_QT)
    const pen pen::invisible(color::black, 1, pen::Style::invisible);
    const pen pen::standard;

    pen::pen (const os::color& color, size_type size, Style style, Cap cap, Join join)
      : m_color(color)
      , m_size(size)
      , m_style(style)
      , m_cap(cap)
      , m_join(join)
    {}

    pen::~pen () = default;

    os::color pen::color () const {
      return m_color;
    }

    pen::size_type pen::size () const {
      return m_size;
    }

    os::size_type pen::os_size () const {
      return core::global::scale_to_native<os::size_type, size_type>(m_size);
    }

    pen::Style pen::style () const {
      return m_style;
    }

    pen::Cap pen::cap () const {
      return m_cap;
    }

    pen::Join pen::join () const {
      return m_join;
    }

    bool pen::operator== (const pen& rhs) const {
      return ((m_color == rhs.m_color) &&
              (m_style == rhs.m_style) &&
              (m_size == rhs.m_size) &&
              (m_cap == rhs.m_cap) &&
              (m_join == rhs.m_join));
    }

#endif // GUIPP_X11 || GUIPP_QT

  }

}
