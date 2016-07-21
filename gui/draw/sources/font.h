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
* @brief     C++ API: font definition
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iosfwd>
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace draw {

    struct /*immutable*/ font {

      static const font& system();
      static const font& system_bold();
      static const font& monospace();
      static const font& serif();
      static const font& sans_serif();

      typedef int size_type;
      typedef core::font_type type;

      enum Thickness {
#ifdef WIN32
        thin = FW_THIN,
        ultraLight = FW_ULTRALIGHT,
        light = FW_LIGHT,
        regular = FW_REGULAR,
        medium = FW_MEDIUM,
        semiBold = FW_SEMIBOLD,
        bold = FW_BOLD,
        ultraBold = FW_ULTRABOLD,
        heavy = FW_HEAVY
#elif X11
        thin,
        ultraLight,
        light,
        regular,
        medium,
        semiBold,
        bold,
        ultraBold,
        heavy
#endif // X11
      };

      font(const std::string& name,
           size_type size,
           Thickness thickness = regular,
           int rotation = 0,
           bool italic = false,
           bool underline = false,
           bool strikeout = false);

      font(const font&);
      ~font();

#ifdef WIN32
      font(core::font_id id);

      operator core::font_id() const;
#endif // WIN32

      std::string name() const;
      size_type size() const;
      Thickness thickness() const;
      int rotation() const;
      bool italic() const;
      bool underline() const;
      bool strikeout() const;

      font with_size(size_type) const;
      font with_thickness(Thickness) const;
      font with_rotation(int) const;
      font with_italic(bool) const;
      font with_underline(bool) const;
      font with_strikeout(bool) const;

      bool operator== (const font&) const;

#ifdef X11
      std::string get_full_name() const;
#endif // X11

    private:
#ifdef WIN32
      core::font_id id;
#endif // X11

      core::font_type info;
    };

    std::ostream& operator<<(std::ostream& out, const font& c);

  }

}
