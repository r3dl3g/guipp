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
#include <gui/draw/brush.h>

#ifdef DEBUG_LOGGING
# include <base/logger.h>
#endif // DEBUG_LOGGING


namespace gui {

  namespace draw {

#ifdef WIN32
    const brush default_brush((os::brush)GetStockObject(WHITE_BRUSH));

    brush::brush (os::brush os_id)
      : id(os_id) {
      if (reinterpret_cast<size_t>(id) > 31) {
        const int expected = sizeof (os::win32::brush_type);
        int result = GetObject(os_id, expected, &info);
# ifdef DEBUG_LOGGING
        if (result != expected) {
          LogDebug << "GetObject result returned unexpected result:" << result << " (expected:" << expected << ")";
        }
# else
        (void)result;
# endif
      }
    }

    brush::brush (const os::color& color, Style style)
      : id(0)
    {
      info.lbColor = color;
      info.lbStyle = style;
      info.lbHatch = 0;
    }

    brush::brush (const brush& rhs)
      : id(reinterpret_cast<size_t>(rhs.id) > 31 ? 0 : rhs.id)
      , info(rhs.info)
    {}

    brush::~brush () {
      destroy();
    }

    void brush::destroy () {
      if (reinterpret_cast<size_t>(id) > 31) {
        DeleteObject(id);
        id = 0;
      }
    }

    brush::operator os::brush() const {
      if (!id) {
        id = CreateBrushIndirect(&info);
      }
      return id;
    }

    os::color brush::color () const {
      return info.lbColor;
    }

    brush::Style brush::style () const {
      return (brush::Style)info.lbStyle;
    }

    brush brush::with_style (Style s) const {
      os::win32::brush_type newType = info;
      newType.lbStyle = s;
      return brush(CreateBrushIndirect(&newType));
    }

    brush brush::with_color (const os::color& c) const {
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
    const brush default_brush;

    brush::brush (const os::color& color, Style style)
      : m_color(color)
      , m_style(style)
    {}

    brush::brush (const brush& rhs)
      : m_color(rhs.m_color)
      , m_style(rhs.m_style)
    {}

    brush::~brush () {}

    os::color brush::color () const {
      return m_color;
    }

    brush::Style brush::style () const {
      return m_style;
    }

    brush brush::with_style (Style s) const {
      return brush(m_color, s);
    }

    brush brush::with_color (const os::color& c) const {
      return brush(c, m_style);
    }

    bool brush::operator== (const brush& rhs) const {
      return ((m_color == rhs.m_color) &&
              (m_style == rhs.m_style));
    }

#endif // X11

  }

}
