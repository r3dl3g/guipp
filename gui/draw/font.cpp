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
#include <locale>

#ifdef X11
# include <vector>
# include <sstream>
# include <X11/Xlib.h>
#endif // X11


// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/logger.h>
#include <gui/draw/font.h>

#ifdef WIN32
# include <base/string_util.h>
#endif // WIN32


namespace std {
  template<typename T>
  std::ostream& operator<< (std::ostream& out,
                            const std::vector<T>& v) {
    out << "[";
    for (auto i : v) {
      out << i << ", ";
    }
    out << "]";
    return out;
  }

}

namespace gui {

  namespace draw {

    template<typename T>
    inline T font_scale (T v) {
      return static_cast<T>((double)v * core::global::get_scale_factor());
    }

    template<typename T>
    inline T font_unscale (T v) {
      return static_cast<T>((double)v / core::global::get_scale_factor());
    }

#ifdef WIN32
    os::font_type get_menu_font () {
      NONCLIENTMETRICS metrics;
      memset(&metrics, 0, sizeof(metrics));
      metrics.cbSize = sizeof (NONCLIENTMETRICS);
      SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof (NONCLIENTMETRICS), &metrics, 0);
      return metrics.lfMenuFont;
    }

    const font& font::system () {
      static font f((os::font)GetStockObject(SYSTEM_FONT));
      return f;
    }

    const font& font::system_bold () {
      static font f = font::system().with_thickness(font::bold);
      return f;
    }

    const font& font::menu () {
      static font f(get_menu_font());
      return f;
    }

    const font& font::monospace () {
      static font f((os::font)GetStockObject(SYSTEM_FIXED_FONT));
      return f;
    }

    const font& font::serif () {
      static font f("Times New Roman", font::system().size());
      return f;
    }

    const font& font::sans_serif () {
      static font f("Arial", font::system().size());
      return f;
    };

    font::font (os::font id)
      : id(id) {
      GetObject(id, sizeof (os::font_type), &info);
    }

    font::font (os::font_type info)
      : id(CreateFontIndirect(&info))
      , info(info)
    {}

    font::font (const std::string& name,
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
      GetObject(id, sizeof (os::font_type), &info);
    }

    font::font (const font& rhs)
      : id(CreateFontIndirect(&rhs.info))
      , info(rhs.info)
    {}

    font::~font () {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
    }

    font::operator os::font() const {
      return id;
    }

    std::string font::name () const {
      return info.lfFaceName;
    }

    font::size_type font::size () const {
      return info.lfHeight;
    }

    font::Thickness font::thickness () const {
      return (Thickness)info.lfWeight;
    }

    int font::rotation () const {
      return info.lfOrientation;
    }

    bool font::italic () const {
      return info.lfItalic != 0;
    }

    bool font::underline () const {
      return info.lfUnderline != 0;
    }

    bool font::strikeout () const {
      return info.lfStrikeOut != 0;
    }

    font::size_type font::line_height () const {
      return info.lfHeight;
    }

    font font::with_size (size_type sz) const {
      os::font_type newType = info;
      newType.lfHeight = sz;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_thickness (Thickness t) const {
      os::font_type newType = info;
      newType.lfWeight = t;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_rotation (int r) const {
      os::font_type newType = info;
      newType.lfOrientation = r;
      newType.lfEscapement = r;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_italic (bool i) const {
      os::font_type newType = info;
      newType.lfItalic = i;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_underline (bool u) const {
      os::font_type newType = info;
      newType.lfUnderline = u;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_strikeout (bool s) const {
      os::font_type newType = info;
      newType.lfStrikeOut = s;
      return font(CreateFontIndirect(&newType));
    }

    bool font::operator== (const font& rhs) const {
      return ((info.lfHeight == rhs.info.lfHeight) &&
              (info.lfEscapement == rhs.info.lfEscapement) &&
              (info.lfOrientation == rhs.info.lfOrientation) &&
              (info.lfWeight == rhs.info.lfWeight) &&
              (info.lfItalic == rhs.info.lfItalic) &&
              (info.lfUnderline == rhs.info.lfUnderline) &&
              (info.lfStrikeOut == rhs.info.lfStrikeOut) &&
              (strcmp(info.lfFaceName, rhs.info.lfFaceName) == 0));
    }

    std::ostream& operator<< (std::ostream& out, const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness() << ", " << f.italic();
      return out;
    }

    core::size font::get_text_size (const std::string& str) const {
      HDC hdc = GetDC(NULL);
      HGDIOBJ old = SelectObject(hdc, id);
      SIZE sz = {0, 0};
      std::wstring wstr = basepp::string::utf8_to_utf16(str);
      GetTextExtentPoint32W(hdc, wstr.c_str(), static_cast<int>(str.length()), &sz);
      SelectObject(hdc, old);
      ReleaseDC(NULL, hdc);
      return core::size(sz);
    }

#endif // WIN32
#ifdef X11
# define STD_FONT_SIZE 10

    const font& font::system () {
      static font f("FreeSans", STD_FONT_SIZE);
      return f;
    }

    const font& font::menu () {
      static font f("FreeSans", STD_FONT_SIZE);
      return f;
    }

    const font& font::system_bold () {
      static font f("FreeSans", STD_FONT_SIZE, font::bold);
      return f;
    }

    const font& font::monospace () {
      static font f("FreeMono", STD_FONT_SIZE);
      return f;
    }

    const font& font::serif () {
      static font f("FreeSerif", STD_FONT_SIZE);
      return f;
    }

    const font& font::sans_serif () {
      static font f("FreeSans", STD_FONT_SIZE);
      return f;
    }

    font::font (os::font id) {
      info = XftFontOpenPattern(core::global::get_instance(), id);
    }

    font::font (os::font_type id)
      : info(id)
    {}

    font::font (const std::string& name,
                font::size_type size,
                font::Thickness thickness,
                int rotation,
                bool italic,
                bool underline,
                bool strikeout)
      : info(nullptr)
    {
      info = XftFontOpen(core::global::get_instance(),
                         core::global::get_screen(),
                         XFT_FAMILY, XftTypeString, name.c_str(),
                         XFT_SIZE, XftTypeDouble, (double)font_scale(size),
                         XFT_WEIGHT, XftTypeInteger, (int)thickness,
                         XFT_SLANT, XftTypeInteger, (italic ? FC_SLANT_ITALIC : 0),
                         NULL);
    }

    font::font (const font& rhs)
      : info(nullptr) {
      info = XftFontCopy(core::global::get_instance(), rhs.info);
    }

    font& font::operator= (const font& rhs) {
      if (this == &rhs) {
        return *this;
      }
      destroy();
      if (rhs.info) {
        info = XftFontCopy(core::global::get_instance(), rhs.info);
      }
      return *this;
    }

    font::~font () {
      destroy();
    }

    void font::destroy () {
      if (info) {
        if (core::global::get_instance()) {
          XftFontClose(core::global::get_instance(), info);
        }
        info = nullptr;
      }
    }

    font::operator os::font() const {
      return (info ? info->pattern : nullptr);
    }

    os::font_type font::font_type () const {
      return info;
    }

    std::string font::name () const {
      if (info) {
        char* name = nullptr;
        if (XftResultMatch == XftPatternGetString(info->pattern, XFT_FAMILY, 0, &name)) {
          return name;
        }
      }
      return std::string();
    }

    font::size_type font::size () const {
      if (info) {
        double sz;
        if (XftResultMatch == XftPatternGetDouble(info->pattern, XFT_SIZE, 0, &sz)) {
          return font_unscale<font::size_type>(sz);
        }
      }
      return STD_FONT_SIZE;
    }

    font::Thickness font::thickness () const {
      if (info) {
        int sz;
        if (XftResultMatch == XftPatternGetInteger(info->pattern, XFT_WEIGHT, 0, &sz)) {
          return (font::Thickness)sz;
        }
      }
      return font::Thickness::regular;
    }

    int font::rotation () const {
      return 0;
    }

    bool font::italic () const {
      if (info) {
        int sz;
        if (XftResultMatch == XftPatternGetInteger(info->pattern, XFT_SLANT, 0, &sz)) {
          return sz != 0;
        }
      }
      return false;
    }

    bool font::underline () const {
      return false;
    }

    bool font::strikeout () const {
      return false;
    }

    font::size_type font::line_height () const {
      if (info) {
        return info->height;
      }
      return STD_FONT_SIZE;
    }

    font font::with_size (size_type sz) const {
      return font(name(), sz, thickness(), rotation(), italic(), underline(), strikeout());
    }

    font font::with_thickness (Thickness t) const {
      return font(name(), size(), t, rotation(), italic(), underline(), strikeout());
    }

    font font::with_rotation (int r) const {
      return *this;
    }

    font font::with_italic (bool i) const {
      font f(name(), size(), thickness(), rotation(), i, underline(), strikeout());
      return f;
    }

    font font::with_underline (bool) const {
      return *this;
    }

    font font::with_strikeout (bool) const {
      return *this;
    }

    bool font::operator== (const font& rhs) const {
      return name() == rhs.name();
    }

    core::size font::get_text_size (const std::string& str) const {
      if (font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(core::global::get_instance(),
                           font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        return core::size(font_unscale(extents.xOff - extents.x), font_unscale(extents.height));
      }
      return core::size::zero;
    }

    std::ostream& operator<< (std::ostream& out,
                              const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness() << ", " << f.italic();
      return out;
    }

#endif // X11

  }

}
