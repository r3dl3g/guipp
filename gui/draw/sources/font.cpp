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

#ifdef WIN32
    const font font::system((core::font_id)GetStockObject(SYSTEM_FONT));
    const font font::system_bold = font::system.with_thickness(font::bold);
    const font font::monospace((core::font_id)GetStockObject(SYSTEM_FIXED_FONT));
    const font font::serif("Times New Roman", font::system.size());
    const font font::sans_serif("Arial", font::system.size());

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

#endif // WIN32
#ifdef X11
    const font font::system((core::font_id)GetStockObject(SYSTEM_FONT));
    const font font::system_bold = font::system.with_thickness(font::bold);
    const font font::monospace((core::font_id)GetStockObject(SYSTEM_FIXED_FONT));
    const font font::serif("Times New Roman", font::system.size());
    const font font::sans_serif("Arial", font::system.size());

    font::font(core::font_id id)
      :type(nullptr)
    {
      type = XQueryFont(core::global::get_instance(), id);
    }

    std::string buildFontName(const std::string& name,
                              font::size_type size,
                              font::Thickness thickness,
                              bool italic) {
      std::ostringstream s;
      s << "-*-" << name << "-";
      switch (thickness) {
        case thin:
          s << "thin";
          break;
        case ultraLight:
        case light:
          s << "light";
          break;
        case regular:
        case medium:
          s << "medium";
          break;
        case semiBold:
        case bold:
        case ultraBold:
        case heavy:
        case medium:
          s << "bold";
          break;
        default:
          s << "*";
      }
      s << "-" << (italic ? "i" : "r") << "-normal-*-" << (size * 10) << "*";
      return s.str();
    }

    void parseFontName(const std::string& full_name,
                       std::string& name,
                       font::size_type& size,
                       font::Thickness& thickness,
                       bool& italic) {
    }

    font::font(const std::string& name,
               font::size_type size,
               font::Thickness thickness,
               int rotation,
               bool italic,
               bool underline,
               bool strikeout)
      : type(nullptr)
    {
      type = XLoadQueryFont(core::global::get_instance(), buildFontName(name, size, thickness, italic).c_str());
    }

    font::font(const font& rhs)
      : type(nullptr) 
    {
      type = XQueryFont(core::global::get_instance(), rhs.type ? rhs.type->fid : 0);
    }

    font::~font() {
      if (type) {
        XFreeFont(core::global::get_instance(), type);
        type = nullptr;
      }
    }

    font::operator core::font_id() const {
      return type ? type->fid : 0;
    }

    std::string font::name() const {
      if (type) {
        unsigned long pid;
        if (XGetFontProperty(type, XA_FONT, &pid)) {
          std::string full_name = XGetAtomName(core::global::get_instance(), (Atom)pid);
          std::string name;
          font::size_type size;
          font::Thickness thickness;
          bool italic;
          parseFontName(full_name, name, size, thickness, italic);
          return name;
        }
      }
      return std::string();
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
      return type.lfUnderline != 0;
    }

    bool font::strikeout() const {
      return type.lfStrikeOut != 0;
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
#endif // X11


    std::ostream& operator<<(std::ostream& out, const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness() << ", " << f.italic();
      return out;
    }

  }

}
