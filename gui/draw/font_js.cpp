/**
 * @copyright (c) 2026-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     font definition
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_JS

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>

#include <logging/logger.h>
#include <util/string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/font.h"

# define STD_FONT_SIZE 10

namespace gui {

  namespace draw {

    const font& font::system () {
      static font f("Verdana", STD_FONT_SIZE);
      return f;
    }

    const font& font::menu () {
      static font f("Verdana", STD_FONT_SIZE);
      return f;
    }

    const font& font::system_bold () {
      static font f("Verdana", STD_FONT_SIZE, font::bold);
      return f;
    }

    const font& font::system_small () {
      static font f("Verdana", STD_FONT_SIZE * 4 / 5);
      return f;
    }

    const font& font::monospace () {
      static font f("Courier New", STD_FONT_SIZE);
      return f;
    }

    const font& font::serif () {
      static font f("Times New Roman", STD_FONT_SIZE);
      return f;
    }

    const font& font::sans_serif () {
      static font f("Verdana", STD_FONT_SIZE);
      return f;
    }
  
    font::font (os::font_type i)
      : info(i)
    {}

    font::font (const std::string& name,
                font::size_type size,
                font::Thickness thickness,
                int rotation,
                bool italic,
                bool underline,
                bool strikeout)
      : info{name, size, thickness, rotation, italic, underline, strikeout}
    {}

    font::font (const font& rhs)
      : info(rhs.info)
    {}

    font::~font ()
    {}

    os::font_type font::font_type () const {
      return info;
    }

    std::string font::name () const {
      return info.name;
    }

    font::size_type font::size () const {
      return info.size;
    }

    font::Thickness font::thickness () const {
      return static_cast<font::Thickness>(info.thickness);
    }

    int font::rotation () const {
      return info.rotation;
    }

    bool font::italic () const {
      return info.italic;
    }

    bool font::underline () const {
      return info.underline;
    }

    bool font::strikeout () const {
      return info.strikeout;
    }

    core::size::type font::line_height () const {
      return core::global::scale_from_native<core::size::type>(native_line_height());
    }

    font::size_type font::native_line_height () const {
      return size() * 4 / 3;
    }

    font font::with_size (size_type sz) const {
      return font(name(), sz, thickness(), rotation(), italic(), underline(), strikeout());
    }

    font font::with_thickness (Thickness t) const {
      return font(name(), size(), t, rotation(), italic(), underline(), strikeout());
    }

    font font::with_rotation (int r) const {
      return font(name(), size(), thickness(), r, italic(), underline(), strikeout());
    }

    font font::with_italic (bool i) const {
      return font(name(), size(), thickness(), rotation(), i, underline(), strikeout());
    }

    font font::with_underline (bool u) const {
      return font(name(), size(), thickness(), rotation(), italic(), u, strikeout());
    }

    font font::with_strikeout (bool s) const {
      return font(name(), size(), thickness(), rotation(), italic(), underline(), s);
    }

    font& font::operator= (const font& rhs) {
      if (this == &rhs) {
        return *this;
      }
      info = rhs.info;
      return *this;
    }

    bool font::operator== (const font& rhs) const {
      return (name() == rhs.name()) 
        && (size() == rhs.size()) 
        && (thickness() == rhs.thickness())
        && (rotation() == rhs.rotation())
        && (italic() == rhs.italic())
        && (underline() == rhs.underline())
        && (strikeout() == rhs.strikeout());
    }

    std::ostream& operator<< (std::ostream& out, const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness() << ", " << f.italic();
      return out;
    }

    core::size font::get_text_size (const std::string& str) const {
      auto h = line_height();
      return core::size(h /2 * str.size(), h);
    }

  }
}
#endif //GUIPP_JS
