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

      const static font system;
      const static font system_bold;
      const static font monospace;
      const static font serif;
      const static font sans_serif;

      typedef int size_type;

      enum Thickness {
        thin = FW_THIN,
        ultraLight = FW_ULTRALIGHT,
        light = FW_LIGHT,
        regular = FW_REGULAR,
        medium = FW_MEDIUM,
        semiBild = FW_SEMIBOLD,
        bold = FW_BOLD,
        ultraBold = FW_ULTRABOLD,
        heavy = FW_HEAVY
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

      inline operator core::font_id() const {
        return id;
      }

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
      
      void swap(font&);

    private:
      font(core::font_id);

      core::font_id id;
      core::font_type type;

    };

    std::ostream& operator<<(std::ostream& out, const font& c);

  }

}
