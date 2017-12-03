/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <gui/core/gui_types.h>


namespace gui {

  namespace draw {

    struct GUIPP_EXPORT /*immutable*/ font {

      static const font& system ();
      static const font& system_bold ();
      static const font& menu ();
      static const font& monospace ();
      static const font& serif ();
      static const font& sans_serif ();

      typedef int size_type;
      typedef os::font_type type;

      enum Thickness {
        thin = IF_WIN32_ELSE(FW_THIN, FC_WEIGHT_THIN),
        ultraLight = IF_WIN32_ELSE(FW_ULTRALIGHT, FC_WEIGHT_ULTRALIGHT),
        light = IF_WIN32_ELSE(FW_LIGHT, FC_WEIGHT_SEMILIGHT),
        regular = IF_WIN32_ELSE(FW_REGULAR, FC_WEIGHT_REGULAR),
        medium = IF_WIN32_ELSE(FW_MEDIUM, FC_WEIGHT_MEDIUM),
        semiBold = IF_WIN32_ELSE(FW_SEMIBOLD, FC_WEIGHT_SEMIBOLD),
        bold = IF_WIN32_ELSE(FW_BOLD, FC_WEIGHT_BOLD),
        ultraBold = IF_WIN32_ELSE(FW_ULTRABOLD, FC_WEIGHT_ULTRABOLD),
        heavy = IF_WIN32_ELSE(FW_HEAVY, FC_WEIGHT_HEAVY)
      };

      font (const std::string& name,
            size_type size,
            Thickness thickness = regular,
            int rotation = 0,
            bool italic = false,
            bool underline = false,
            bool strikeout = false);

      font (const font&);
      ~font ();

      font& operator= (const font& rhs);

      font (os::font id);
      font (os::font_type id);

      operator os::font () const;
      os::font_type font_type () const;

      std::string name () const;
      size_type size () const;
      Thickness thickness () const;
      int rotation () const;
      bool italic () const;
      bool underline () const;
      bool strikeout () const;

      size_type line_height () const;

      font with_size(size_type) const;
      font with_thickness(Thickness) const;
      font with_rotation (int) const;
      font with_italic (bool) const;
      font with_underline (bool) const;
      font with_strikeout (bool) const;

      bool operator== (const font&) const;

      core::size get_text_size (const std::string& text) const;

    private:
      void destroy ();
#ifdef WIN32
      os::font id;
#endif // WIN32
      os::font_type info;
    };

    GUIPP_EXPORT std::ostream& operator<< (std::ostream& out, const font& c);

  }

}
