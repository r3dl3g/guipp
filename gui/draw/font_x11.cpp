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
 * @brief     font definition
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>

#include <vector>
#include <X11/Xlib.h>

#include <logging/logger.h>
#include <util/string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/font.h"

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

# define STD_FONT_SIZE 10

namespace gui {

  namespace draw {

#if defined(BUILD_FOR_ARM) || defined(NO_FONT_SCALE)
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
#ifdef GUIPP_USE_XFT
          os::font_type info_ = XftFontOpen(core::global::get_instance(),
                                           core::global::x11::get_screen(),
                                           XFT_FAMILY, XftTypeString, "Courier",
                                           XFT_SIZE, XftTypeDouble, 10.0,
                                           XFT_WEIGHT, XftTypeInteger, FC_WEIGHT_REGULAR,
                                           XFT_SLANT, XftTypeInteger, FC_SLANT_ROMAN,
                                           NULL);
          double dpi;
          XftPatternGetDouble(info_->pattern, XFT_DPI, 0, &dpi);
          s_font_scale = 96 / dpi * core::global::get_scale_factor();
          XftFontClose(core::global::get_instance(), info_);
#else
          s_font_scale = core::global::get_scale_factor();
#endif // GUIPP_USE_XFT
          logging::debug() << "Font scale = " << s_font_scale;
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

    const font& font::system_small () {
      static font f("FreeSans", STD_FONT_SIZE * 4 / 5);
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

    font::font ()
      : info_(nullptr)
    {}

    font::font (os::font id) {
#ifdef GUIPP_USE_XFT
      info_ = XftFontOpenPattern(core::global::get_instance(), id);
#else
      info_ = XQueryFont(core::global::get_instance(), id);
#endif // GUIPP_USE_XFT
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
      s << "-" << (italic ? "i" : "r") << "-*--" << size << "-*";

      std::string str = s.str();
      logging::debug() << "Font name:" << str;
      //std::transform(str.begin(), str.end(), str.begin(), tolower);
      return str;
    }

    font::font (os::font_type id)
      : info_(id)
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
      logging::debug() << "Font:'" << full_name << "' parts:" << strs;
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
      if (size && (strs.size() > 8) && (!strs[8].empty())) {
        logging::debug() << "Font size:" << strs[8];
        *size = std::stoi(strs[8]) / 10;
      } else if (size && (strs.size() > 7) && (!strs[7].empty())) {
        logging::debug() << "Font size:" << strs[7];
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
      : info_(nullptr)
    {
      init_font_scale();
#ifdef GUIPP_USE_XFT
      info_ = XftFontOpen(core::global::get_instance(),
                         core::global::x11::get_screen(),
                         XFT_FAMILY, XftTypeString, name.c_str(),
                         XFT_SIZE, XftTypeDouble, (double)font_scale(size),
                         XFT_WEIGHT, XftTypeInteger, (int)thickness,
                         XFT_SLANT, XftTypeInteger, (italic ? FC_SLANT_ITALIC : 0),
                         NULL);
#else
      auto fs = static_cast<double>(font_scale(size));
      std::string full_name = buildFontName(name, fs, thickness, italic);
      os::font_type f = XLoadQueryFont(core::global::get_instance(), full_name.c_str());
      if (!f) {
        full_name = buildFontName("fixed", fs, thickness, italic);
        f = XLoadQueryFont(core::global::get_instance(), full_name.c_str());
        if (!f) {
          f = XLoadQueryFont(core::global::get_instance(), "fixed");
        }
      }
      info_ = f;
      logging::debug() << "Load font:" << get_full_name();
#endif // GUIPP_USE_XFT
    }

    font::font (const font& rhs)
      : info_(nullptr) {
#ifdef GUIPP_USE_XFT
      info_ = XftFontCopy(core::global::get_instance(), rhs.info_);
#else
      info_ = XLoadQueryFont(core::global::get_instance(), rhs.get_full_name().c_str());
#endif // GUIPP_USE_XFT
    }

    font& font::operator= (const font& rhs) {
      if (this == &rhs) {
        return *this;
      }
      destroy();
      if (rhs.info_) {
#ifdef GUIPP_USE_XFT
        info_ = XftFontCopy(core::global::get_instance(), rhs.info_);
#else
        info_ = XLoadQueryFont(core::global::get_instance(), rhs.get_full_name().c_str());
#endif // GUIPP_USE_XFT
      }
      return *this;
    }

    bool font::is_valid () const {
      return info_ != nullptr;
    }

    font::~font () {
      destroy();
    }

    void font::destroy () {
      if (info_) {
        if (core::global::get_instance()) {
#ifdef GUIPP_USE_XFT
          XftFontClose(core::global::get_instance(), info_);
#else
          XFreeFont(core::global::get_instance(), info_);
#endif // GUIPP_USE_XFT
        }
        info_ = nullptr;
      }
    }

    font::operator os::font() const {
#ifdef GUIPP_USE_XFT
      return (info_ ? info_->pattern : nullptr);
#else
      return info_ ? info_->fid : 0;
#endif // GUIPP_USE_XFT
    }

    os::font_type font::font_type () const {
      return info_;
    }

#ifndef GUIPP_USE_XFT
    std::string font::get_full_name() const {
      if (info_) {
        unsigned long pid;
        if (XGetFontProperty(info_, XA_FONT, &pid)) {
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
#endif // GUIPP_USE_XFT

    std::string font::name () const {
      if (info_) {
#ifdef GUIPP_USE_XFT
        char* name = nullptr;
        if (XftResultMatch == XftPatternGetString(info_->pattern, XFT_FAMILY, 0, &name)) {
          return name;
        }
#else
        std::string name;
        parseFontName(get_full_name(), &name);
        return name;
#endif // GUIPP_USE_XFT
      }
      return std::string();
    }

    font::size_type font::size () const {
      if (info_) {
        double sz;
#ifdef GUIPP_USE_XFT
        if (XftResultMatch == XftPatternGetDouble(info_->pattern, XFT_SIZE, 0, &sz)) {
          return static_cast<font::size_type>(sz);
        }
#else
      font::size_type size = -1;
      parseFontName(get_full_name(), nullptr, &size);
      return size;
#endif // GUIPP_USE_XFT
      }
      return STD_FONT_SIZE;
    }

    font::Thickness font::thickness () const {
      if (info_) {
        int sz;
#ifdef GUIPP_USE_XFT
        if (XftResultMatch == XftPatternGetInteger(info_->pattern, XFT_WEIGHT, 0, &sz)) {
          return font::Thickness(sz);
        }
#else
      font::Thickness thickness = Thickness::regular;
      parseFontName(get_full_name(), nullptr, nullptr, &thickness);
      return thickness;
#endif // GUIPP_USE_XFT
      }
      return font::Thickness::regular;
    }

    int font::rotation () const {
      return 0;
    }

    bool font::italic () const {
      if (info_) {
        int sz;
#ifdef GUIPP_USE_XFT
        if (XftResultMatch == XftPatternGetInteger(info_->pattern, XFT_SLANT, 0, &sz)) {
          return sz != 0;
        }
#else
      bool italic = false;
      parseFontName(get_full_name(), nullptr, nullptr, nullptr, &italic);
      return italic;
#endif // GUIPP_USE_XFT
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
      return core::global::scale_from_native<core::size::type>(native_line_height());
    }

    font::size_type font::native_line_height () const {
      if (info_) {
#ifdef GUIPP_USE_XFT
        return info_->height;
#else
        return info_->ascent + info_->descent;
#endif // GUIPP_USE_XFT
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
#ifdef GUIPP_USE_XFT
        XGlyphInfo extents;
        XftTextExtentsUtf8(core::global::get_instance(),
                           font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        return core::size(core::global::scale_from_native<core::size::type>(extents.width), core::global::scale_from_native<core::size::type>(extents.height));
#else
        return core::size(XTextWidth(font_type(), str.c_str(), str.size()));
#endif // GUIPP_USE_XFT
      }
      return core::size::zero;
    }

    std::ostream& operator<< (std::ostream& out,
                              const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness() << ", " << f.italic();
      return out;
    }
  }
}
#endif // GUIPP_X11
