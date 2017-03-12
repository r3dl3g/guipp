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
#ifdef X11
#include <vector>
#include <sstream>
#endif // X11
#include <locale>
#include <logger.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "font.h"

#ifdef X11
#include <boost/algorithm/string.hpp>
#endif // X11

namespace std {
  template<typename T>
  std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    out << "[";
    for(auto i : v) {
      out << i << ", ";
    }
    out << "]";
    return out;
  }
}

namespace gui {

  namespace draw {

#ifdef WIN32
    const font& font::system() {
        static font f((core::font_id)GetStockObject(SYSTEM_FONT));
        return f;
    }
    const font& font::system_bold() {
        static font f = font::system().with_thickness(font::bold);
        return f;
    }
    const font& font::monospace() {
        static font f((core::font_id)GetStockObject(SYSTEM_FIXED_FONT));
        return f;
    }
    const font& font::serif() {
        static font f("Times New Roman", font::system().size());
        return f;
    }
    const font& font::sans_serif() {
        static font f("Arial", font::system().size());
        return f;
    };

    font::font(core::font_id id)
      : id(id) {
      GetObject(id, sizeof(core::font_type), &info);
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
      GetObject(id, sizeof(core::font_type), &info);
    }

    font::font(const font& rhs)
      : id(CreateFontIndirect(&rhs.info))
      , info(rhs.info)
    {}

    font::~font() {
      if (id) {
        DeleteObject(id);
        id = 0;
      }
    }

    font::operator core::font_id() const {
      return id;
    }

    std::string font::name() const {
      return info.lfFaceName;
    }

    font::size_type font::size() const {
      return info.lfHeight;
    }

    font::Thickness font::thickness() const {
      return (Thickness)info.lfWeight;
    }

    int font::rotation() const {
      return info.lfOrientation;
    }

    bool font::italic() const {
      return info.lfItalic != 0;
    }

    bool font::underline() const {
      return info.lfUnderline!= 0;
    }

    bool font::strikeout() const {
      return info.lfStrikeOut!= 0;
    }

    font font::with_size(size_type sz) const {
      core::font_type newType = info;
      newType.lfHeight = sz;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_thickness(Thickness t) const {
      core::font_type newType = info;
      newType.lfWeight = t;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_rotation(int r) const {
      core::font_type newType = info;
      newType.lfOrientation = r;
      newType.lfEscapement = r;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_italic(bool i) const {
      core::font_type newType = info;
      newType.lfItalic = i;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_underline(bool u) const {
      core::font_type newType = info;
      newType.lfUnderline = u;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_strikeout(bool s) const {
      core::font_type newType = info;
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

    std::ostream& operator<<(std::ostream& out, const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness() << ", " << f.italic();
      return out;
    }

#endif // WIN32
#ifdef X11
    #define STD_FONT_SIZE 12

    const font& font::system() {
        static font f("fixed", STD_FONT_SIZE);
        return f;
    }
    const font& font::system_bold() {
        static font f = font::system().with_thickness(font::bold);
        return f;
    }
    const font& font::monospace() {
        static font f("fixed", STD_FONT_SIZE);
        return f;
    }
    const font& font::serif() {
        static font f("clean", STD_FONT_SIZE);
        return f;
    }
    const font& font::sans_serif() {
        static font f("clean", STD_FONT_SIZE);
        return f;
    }

    font::font(core::font_id id)
      :info(nullptr)
    {
      info = XQueryFont(core::global::get_instance(), id);
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

    std::vector<std::string> tokenize(const std::string& full_name, const std::string& delemiter) {
      std::vector<std::string> strs;
      boost::split(strs, full_name, boost::is_any_of(delemiter));
      return strs;
    }

    std::string merge(const std::vector<std::string>& vec, const std::string& delemiter) {
      std::ostringstream oss;
      if (!vec.empty()) {
        std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<std::string>(oss, delemiter.c_str()));
        // add the last element with no delimiter
        oss << vec.back();
      }
      return oss.str();
    }

    void parseFontName(const std::string& full_name,
                       std::string* name = nullptr,
                       font::size_type* size = nullptr,
                       font::Thickness* thickness = nullptr,
                       bool* italic = nullptr) {

      std::vector<std::string> strs = tokenize(full_name, "-");
      LogDebug << "Font:'" << full_name << "' parts:" << strs;
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
        LogDebug << "Font size:" << strs[8];
        *size = std::stoi(strs[8]) / 10;
      } else if (size && (strs.size() > 7) && (strs[7].size() > 0)) {
        LogDebug << "Font size:" << strs[7];
        *size = std::stoi(strs[7]);
      }
    }

    font::font(const std::string& name,
               font::size_type size,
               font::Thickness thickness,
               int rotation,
               bool italic,
               bool underline,
               bool strikeout)
      : info(nullptr)
    {
      std::string full_name = buildFontName(name, size, thickness, italic);
      LogDebug << "Load Query Font:'" << full_name << "'";
      core::font_type f = XLoadQueryFont(core::global::get_instance(), full_name.c_str());
      if (!f) {
        f = XLoadQueryFont(core::global::get_instance(), "fixed");
      }
      info = f;
    }

    font::font(const font& rhs)
      : info(nullptr)
    {
      core::font_type f = XLoadQueryFont(core::global::get_instance(), rhs.get_full_name().c_str());
      info = f;
    }

    font& font::operator= (const font& rhs) {
      if (this == &rhs) {
        return *this;
      }
      core::font_type f = XLoadQueryFont(core::global::get_instance(), rhs.get_full_name().c_str());
      info = f;
      return *this;
    }

    font::~font() {
      if (info) {
        if (core::global::get_instance()) {
          XFreeFont(core::global::get_instance(), info);
        }
        info = nullptr;
      }
    }

    font::operator core::font_id() const {
      return info ? info->fid : 0;
    }

    core::font_type font::font_type () const {
      return info;
    }

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

    std::string font::name() const {
      std::string name;
      parseFontName(get_full_name(), &name);
      return name;
    }

    font::size_type font::size() const {
      font::size_type size = -1;
      parseFontName(get_full_name(), nullptr, &size);
      return size;
    }

    font::Thickness font::thickness() const {
      font::Thickness thickness = Thickness::regular;
      parseFontName(get_full_name(), nullptr, nullptr, &thickness);
      return thickness;
    }

    int font::rotation() const {
      return 0;
    }

    bool font::italic() const {
      bool italic = false;
      parseFontName(get_full_name(), nullptr, nullptr, nullptr, &italic);
      return italic;
    }

    bool font::underline() const {
      return false;
    }

    bool font::strikeout() const {
      return false;
    }

    font font::with_size(size_type sz) const {
      std::string name;
      font::Thickness thickness = Thickness::regular;
      bool italic = false;
      parseFontName(get_full_name(), &name, nullptr, &thickness, &italic);
      return font(name, sz, thickness, 0, italic);
    }

    font font::with_thickness(Thickness t) const {
      std::string name;
      font::size_type size = 10;
      bool italic = false;
      parseFontName(get_full_name(), &name, &size, nullptr, &italic);
      return font(name, size, t, 0, italic);
    }

    font font::with_rotation(int r) const {
      return *this;
    }

    font font::with_italic(bool i) const {
      std::string name;
      font::size_type size = 10;
      font::Thickness thickness = Thickness::regular;
      parseFontName(get_full_name(), &name, &size, &thickness);
      return font(name, size, thickness, 0, i);
    }

    font font::with_underline(bool) const {
      return *this;
    }

    font font::with_strikeout(bool) const {
      return *this;
    }

    bool font::operator== (const font& rhs) const {
      return name() == rhs.name();
    }

    std::ostream& operator<<(std::ostream& out, const font& f) {
      std::string name;
      font::size_type size;
      font::Thickness thickness;
      bool italic;
      parseFontName(f.get_full_name(), &name, &size, &thickness, &italic);
      out << name << ", " << size << ", " << thickness << ", " << italic;
      return out;
    }
#endif // X11

  }

}
