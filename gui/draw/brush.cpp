/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     brush for filled areas
 *
 * @license   MIT license. See accompanying file LICENSE.
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
# include <logging/logger.h>
#endif // DEBUG_LOGGING


namespace gui {

  namespace draw {

#ifdef GUIPP_WIN
    const brush brush::invisible((os::brush)GetStockObject(NULL_BRUSH));
    const brush brush::standard((os::brush)GetStockObject(WHITE_BRUSH));

    brush::brush (os::brush os_id)
      : id(os_id) {
      if (reinterpret_cast<size_t>(id) > 31) {
        const int expected = sizeof (os::win32::brush_type);
        int result = GetObject(os_id, expected, &info);
# ifdef DEBUG_LOGGING
        if (result != expected) {
          logging::debug() << "GetObject result returned unexpected result:" << result << " (expected:" << expected << ")";
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
      info.lbStyle = static_cast<UINT>(style);
      info.lbHatch = 0;
    }

    brush::brush (const brush& rhs)
      : id(0)
      , info(rhs.info)
    {
    }

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
      newType.lbStyle = static_cast<UINT>(s);
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

#endif // GUIPP_WIN

#if defined(GUIPP_X11) || defined(GUIPP_QT)
    const brush brush::invisible(color::black, brush::Style::invisible);
    const brush brush::standard;

    brush::brush (const os::color& color, Style style)
      : m_color(color)
      , m_style(style)
    {}

    brush::brush (const brush& rhs) = default;

    brush::~brush () = default;

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
#endif // GUIPP_X11 || GUIPP_QT

  }

}
