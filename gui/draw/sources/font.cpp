/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
#include "font.h"


namespace gui {

	namespace draw {

    const font font::system((core::font_id)GetStockObject(SYSTEM_FONT));
    const font font::system_bold = font::system.with_thickness(font::bold);
    const font font::monospace((core::font_id)GetStockObject(SYSTEM_FIXED_FONT));
    const font font::serif("Times New Roman", 10);
    const font font::sans_serif("Arial", 10);

    font::font(core::font_id id)
      :id(id)
    {
      GetObject(id, sizeof(core::font_type), &type);
    }

    font::font(const std::string& name,
               font::size_type size,
               font::Thickness thickness,
               int rotation,
               bool italic,
               bool underline,
               bool strikeout) 
      : id(CreateFont(size, 0, rotation, rotation, thickness, italic, underline, strikeout,
      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
      name.c_str()))
    {
      GetObject(id, sizeof(core::font_type), &type);
    }

    font::font(const font& rhs) 
      : id(CreateFontIndirect(&rhs.type))
      , type(rhs.type)
    {}

    font::~font() {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
    }

    std::string font::name() const {
      return type.lfFaceName;
    }

    font::size_type font::size() const {
      return type.lfHeight;
    }

    font::Thickness font::thickness() const {
      return (Thickness)type.lfWeight;
    }

    int font::rotation() const {
      return type.lfOrientation;
    }

    bool font::italic() const {
      return type.lfItalic != 0;
    }

    bool font::underline() const {
      return type.lfUnderline!= 0;
    }

    bool font::strikeout() const {
      return type.lfStrikeOut!= 0;
    }

    font font::with_size(size_type sz) const {
      core::font_type newType = type;
      newType.lfHeight = sz;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_thickness(Thickness t) const {
      core::font_type newType = type;
      newType.lfWeight = t;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_rotation(int r) const {
      core::font_type newType = type;
      newType.lfOrientation = r;
      newType.lfEscapement = r;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_italic(bool i) const {
      core::font_type newType = type;
      newType.lfItalic = i;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_underline(bool u) const {
      core::font_type newType = type;
      newType.lfUnderline = u;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_strikeout(bool s) const {
      core::font_type newType = type;
      newType.lfStrikeOut = s;
      return font(CreateFontIndirect(&newType));
    }

    bool font::operator== (const font& rhs) const {
      return ((type.lfHeight == rhs.type.lfHeight) &&
              (type.lfEscapement == rhs.type.lfEscapement) &&
              (type.lfOrientation == rhs.type.lfOrientation) &&
              (type.lfWeight == rhs.type.lfWeight) &&
              (type.lfItalic == rhs.type.lfItalic) &&
              (type.lfUnderline == rhs.type.lfUnderline) &&
              (type.lfStrikeOut == rhs.type.lfStrikeOut) &&
              (strcmp(type.lfFaceName, rhs.type.lfFaceName) == 0));
    }

    void font::swap(font& rhs) {
      core::font_id id_ = id;
      core::font_type type_ = type;
      id = rhs.id;
      type = rhs.type;
      rhs.id = id_;
      rhs.type = type_;
    }

    std::ostream& operator<<(std::ostream& out, const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness() << ", " << f.italic();
      return out;
    }

  }

}
