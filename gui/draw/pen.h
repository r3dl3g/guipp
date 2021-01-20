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
 * @brief     C++ API: pen definition
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

    struct GUIPP_DRAW_EXPORT /*immutable*/ pen {

#if defined(GUIPP_WIN) || defined(GUIPP_X11) || defined(GUIPP_X11) || defined(GUIPP_QT)
      typedef float size_type;
#endif // GUIPP_WIN | GUIPP_X11

      enum struct Style : int {
        solid =       IF_WIN32_X11_QT_ELSE(PS_SOLID,          LineSolid,              Qt::PenStyle::SolidLine,      1),
        dash =        IF_WIN32_X11_QT_ELSE(PS_DASH,           LineOnOffDash,          Qt::PenStyle::DashLine,       2),
        dot =         IF_WIN32_X11_QT_ELSE(PS_DOT,            LineOnOffDash | 0x010,  Qt::PenStyle::DotLine,        3),
        dashDot =     IF_WIN32_X11_QT_ELSE(PS_DASHDOT,        LineOnOffDash | 0x020,  Qt::PenStyle::DashDotLine,    4),
        dashDotDot =  IF_WIN32_X11_QT_ELSE(PS_DASHDOTDOT,     LineOnOffDash | 0x030,  Qt::PenStyle::DashDotDotLine, 5),
        hairLine =    IF_WIN32_X11_QT_ELSE(PS_NULL,           LineSolid | 0x040,      Qt::PenStyle::SolidLine,      6),
        insideFrame = IF_WIN32_X11_QT_ELSE(PS_INSIDEFRAME,    LineSolid | 0x050,      Qt::PenStyle::SolidLine,      7),
        invisible =   IF_WIN32_X11_QT_ELSE(PS_NULL,           0x0,                    Qt::PenStyle::NoPen,          0)
      };

      enum struct Cap : int {
        flat =        IF_WIN32_X11_QT_ELSE(PS_ENDCAP_FLAT,    CapButt,                Qt::PenCapStyle::FlatCap,     1),
        round =       IF_WIN32_X11_QT_ELSE(PS_ENDCAP_ROUND,   CapRound,               Qt::PenCapStyle::RoundCap,    2),
        square =      IF_WIN32_X11_QT_ELSE(PS_ENDCAP_SQUARE,  CapProjecting,          Qt::PenCapStyle::SquareCap,   3)
      };

      enum struct Join : int {
        miter =       IF_WIN32_X11_QT_ELSE(PS_JOIN_MITER,     JoinMiter,              Qt::PenJoinStyle::MiterJoin,  1),
        round =       IF_WIN32_X11_QT_ELSE(PS_JOIN_ROUND,     JoinRound,              Qt::PenJoinStyle::RoundJoin,  2),
        bevel =       IF_WIN32_X11_QT_ELSE(PS_JOIN_BEVEL,     JoinBevel,              Qt::PenJoinStyle::BevelJoin,  3)
      };

      static const pen invisible;
      static const pen standard;

      pen (const os::color& = color::black, size_type = 1, Style = Style::solid, Cap = Cap::square, Join = Join::miter);
      pen (const pen&);
      ~pen ();

#ifdef GUIPP_WIN
      pen (os::win32::pen);

      inline operator os::win32::pen () const {
        return id;
      }

      void recreate (size_type = 1);

#endif // GUIPP_WIN

      os::color color () const;
      size_type size () const;
      os::size_type os_size () const;
      Style style () const;
      Cap cap () const;
      Join join () const;

      pen with_color (const os::color&) const;
      pen with_size (size_type) const;
      pen with_os_size (os::size_type) const;
      pen with_style (Style) const;
      pen with_cap (Cap) const;
      pen with_join (Join) const;

      bool operator== (const pen&) const;

    private:
#ifdef GUIPP_WIN
      os::win32::pen id;
      os::win32::pen_type info;
#endif // GUIPP_WIN
#if defined(GUIPP_X11) || defined(GUIPP_QT)
      const os::color m_color;
      const size_type m_size;
      const Style m_style;
      const Cap m_cap;
      const Join m_join;
#endif // GUIPP_X11 || GUIPP_QT

    };

  }

}
