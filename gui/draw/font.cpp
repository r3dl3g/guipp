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
#include <logging/logger.h>
#include <gui/draw/font.h>
#include <util/string_util.h>
#

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

#if defined(BUILD_FOR_ARM) || defined(WIN32) || defined(NO_FONT_SCALE)
    template<typename T>
    inline T font_scale (T v) {
      return static_cast<T>((double)v * core::global::get_scale_factor());
    }

    template<typename T>
    inline T font_unscale (T v) {
      return static_cast<T>((double)v / core::global::get_scale_factor());
    }

    void init_font_scale () {}
#else
    namespace {
      double s_font_scale = 0.0;

      void init_font_scale () {
        if (s_font_scale == 0.0) {
#ifdef USE_XFT
          os::font_type info = XftFontOpen(core::global::get_instance(),
                                           core::global::x11::get_screen(),
                                           XFT_FAMILY, XftTypeString, "Courier",
                                           XFT_SIZE, XftTypeDouble, 10.0,
                                           XFT_WEIGHT, XftTypeInteger, FC_WEIGHT_REGULAR,
                                           XFT_SLANT, XftTypeInteger, FC_SLANT_ROMAN,
                                           NULL);
          double dpi;
          XftPatternGetDouble(info->pattern, XFT_DPI, 0, &dpi);
          s_font_scale = 96 / dpi * core::global::get_scale_factor();
          XftFontClose(core::global::get_instance(), info);
#else
          s_font_scale = core::global::get_scale_factor();
#endif // USE_XFT
          clog::debug() << "Font scale = " << s_font_scale;
        }
      }
    }

    template<typename T>
    inline T font_scale (T v) {
      return static_cast<T>(v * s_font_scale);
    }

    template<typename T>
    inline T font_unscale (T v) {
      return static_cast<T>(v / s_font_scale);
    }
#endif

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

    core::size::type font::line_height () const {
      return core::global::scale<core::size::type>(native_line_height());
    }

    font::size_type font::native_line_height () const {
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
      std::wstring wstr = util::string::utf8_to_utf16(str);
      GetTextExtentPoint32W(hdc, wstr.c_str(), static_cast<int>(str.length()), &sz);
      SelectObject(hdc, old);
      ReleaseDC(NULL, hdc);
      return core::size(core::global::scale<core::size::type>(sz.cx), core::global::scale<core::size::type>(sz.cy));
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
#ifdef USE_XFT
      info = XftFontOpenPattern(core::global::get_instance(), id);
#else
      info = XQueryFont(core::global::get_instance(), id);
#endif // USE_XFT
    }

    std::string buildFontName(const std::string& name,
                              font::size_type size,
                              font::Thickness thickness,
                              bool italic) {
      std::ostringstream s;
      s << "-*-" << name << "-";
      switch (thickness) {
        case font::Thickness::thin:
          s << "thin";
          break;
        case font::Thickness::ultraLight:
        case font::Thickness::light:
          s << "light";
          break;
        case font::Thickness::regular:
        case font::Thickness::medium:
          s << "medium";
          break;
        case font::Thickness::semiBold:
        case font::Thickness::bold:
        case font::Thickness::ultraBold:
        case font::Thickness::heavy:
          s << "bold";
          break;
        default:
          s << "*";
      }
      s << "-" << (italic ? "i" : "r") << "-normal-*-" << (size * 10) << "-*";

      std::string str = s.str();
      //std::transform(str.begin(), str.end(), str.begin(), tolower);
      return str;
    }

    font::font (os::font_type id)
      : info(id)
    {}

    std::vector<std::string> tokenize (const std::string& full_name) {
      return util::string::split<'-'>(full_name);
    }

    void parseFontName(const std::string& full_name,
                       std::string* name = nullptr,
                       font::size_type* size = nullptr,
                       font::Thickness* thickness = nullptr,
                       bool* italic = nullptr) {

      std::vector<std::string> strs = tokenize(full_name);
      clog::debug() << "Font:'" << full_name << "' parts:" << strs;
      if (name && strs.size() > 2) {
        *name = strs[2];
      }
      if (thickness && (strs.size() > 3)) {
        const std::string& thck = strs[3];
        if (thck == "thin") {
          *thickness = font::Thickness::thin;
        } else if (thck == "light") {
          *thickness = font::Thickness::light;
        } else if (thck == "medium") {
          *thickness = font::Thickness::medium;
        } else if (thck == "bold") {
          *thickness = font::Thickness::bold;
        } else {
          *thickness = font::Thickness::regular;
        }
      }
      if (italic && (strs.size() > 4)) {
        *italic = (strs[4] == "i") || (strs[4] == "I");
      }
      if (size && (strs.size() > 8) && (strs[8].size() > 0)) {
        clog::debug() << "Font size:" << strs[8];
        *size = std::stoi(strs[8]) / 10;
      } else if (size && (strs.size() > 7) && (strs[7].size() > 0)) {
        clog::debug() << "Font size:" << strs[7];
        *size = std::stoi(strs[7]);
      }
    }

    font::font (const std::string& name,
                font::size_type size,
                font::Thickness thickness,
                int rotation,
                bool italic,
                bool underline,
                bool strikeout)
      : info(nullptr)
    {
      init_font_scale();
#ifdef USE_XFT
      info = XftFontOpen(core::global::get_instance(),
                         core::global::x11::get_screen(),
                         XFT_FAMILY, XftTypeString, name.c_str(),
                         XFT_SIZE, XftTypeDouble, (double)font_scale(size),
                         XFT_WEIGHT, XftTypeInteger, (int)thickness,
                         XFT_SLANT, XftTypeInteger, (italic ? FC_SLANT_ITALIC : 0),
                         NULL);
#else
      std::string full_name = buildFontName(name, (double)font_scale(size), thickness, italic);
      clog::debug() << "Load Query Font:'" << full_name << "'";
      os::font_type f = XLoadQueryFont(core::global::get_instance(), full_name.c_str());
      if (!f) {
        f = XLoadQueryFont(core::global::get_instance(), "fixed");
      }
      info = f;
#endif // USE_XFT
    }

    font::font (const font& rhs)
      : info(nullptr) {
#ifdef USE_XFT
      info = XftFontCopy(core::global::get_instance(), rhs.info);
#else
      info = XLoadQueryFont(core::global::get_instance(), rhs.get_full_name().c_str());
#endif // USE_XFT
    }

    font& font::operator= (const font& rhs) {
      if (this == &rhs) {
        return *this;
      }
      destroy();
      if (rhs.info) {
#ifdef USE_XFT
        info = XftFontCopy(core::global::get_instance(), rhs.info);
#else
        info = XLoadQueryFont(core::global::get_instance(), rhs.get_full_name().c_str());
#endif // USE_XFT
      }
      return *this;
    }

    font::~font () {
      destroy();
    }

    void font::destroy () {
      if (info) {
        if (core::global::get_instance()) {
#ifdef USE_XFT
          XftFontClose(core::global::get_instance(), info);
#else
          XFreeFont(core::global::get_instance(), info);
#endif // USE_XFT
        }
        info = nullptr;
      }
    }

    font::operator os::font() const {
#ifdef USE_XFT
      return (info ? info->pattern : nullptr);
#else
      return info ? info->fid : 0;
#endif // USE_XFT
    }

    os::font_type font::font_type () const {
      return info;
    }

#ifndef USE_XFT
    std::string font::get_full_name() const {
      if (info) {
        unsigned long pid;
        if (XGetFontProperty(info, XA_FONT, &pid)) {
          std::string full_name;
          char* name = XGetAtomName(core::global::get_instance(), (Atom)pid);
          if (name) {
            full_name = name;
            XFree(name);
          }
          return full_name;
        }
      }
      return std::string();
    }
#endif // USE_XFT

    std::string font::name () const {
      if (info) {
#ifdef USE_XFT
        char* name = nullptr;
        if (XftResultMatch == XftPatternGetString(info->pattern, XFT_FAMILY, 0, &name)) {
          return name;
        }
#else
        std::string name;
        parseFontName(get_full_name(), &name);
        return name;
#endif // USE_XFT
      }
      return std::string();
    }

    font::size_type font::size () const {
      if (info) {
        double sz;
#ifdef USE_XFT
        if (XftResultMatch == XftPatternGetDouble(info->pattern, XFT_SIZE, 0, &sz)) {
          return static_cast<font::size_type>(sz);
        }
#else
      font::size_type size = -1;
      parseFontName(get_full_name(), nullptr, &size);
      return size;
#endif // USE_XFT
      }
      return STD_FONT_SIZE;
    }

    font::Thickness font::thickness () const {
      if (info) {
        int sz;
#ifdef USE_XFT
        if (XftResultMatch == XftPatternGetInteger(info->pattern, XFT_WEIGHT, 0, &sz)) {
          return font::Thickness(sz);
        }
#else
      font::Thickness thickness = Thickness::regular;
      parseFontName(get_full_name(), nullptr, nullptr, &thickness);
      return thickness;
#endif // USE_XFT
      }
      return font::Thickness::regular;
    }

    int font::rotation () const {
      return 0;
    }

    bool font::italic () const {
      if (info) {
        int sz;
#ifdef USE_XFT
        if (XftResultMatch == XftPatternGetInteger(info->pattern, XFT_SLANT, 0, &sz)) {
          return sz != 0;
        }
#else
      bool italic = false;
      parseFontName(get_full_name(), nullptr, nullptr, nullptr, &italic);
      return italic;
#endif // USE_XFT
      }
      return false;
    }

    bool font::underline () const {
      return false;
    }

    bool font::strikeout () const {
      return false;
    }

    core::size::type font::line_height () const {
      return core::global::scale<core::size::type>(native_line_height());
    }

    font::size_type font::native_line_height () const {
      if (info) {
#ifdef USE_XFT
        return info->height;
#else
        return info->ascent + info->descent;
#endif // USE_XFT
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
#ifdef USE_XFT
        XGlyphInfo extents;
        XftTextExtentsUtf8(core::global::get_instance(),
                           font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        return core::size(core::global::scale<core::size::type>(extents.width), core::global::scale<core::size::type>(extents.height));
#else
        return core::size(XTextWidth(font_type(), str.c_str(), str.size()));
#endif // USE_XFT
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
