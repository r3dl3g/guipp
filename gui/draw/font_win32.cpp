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

 #ifdef GUIPP_WIN

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

namespace gui {

  namespace draw {

    os::font_type get_menu_font () {
      NONCLIENTMETRICS metrics;
      memset(&metrics, 0, sizeof(metrics));
      metrics.cbSize = sizeof (NONCLIENTMETRICS);
      SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof (NONCLIENTMETRICS), &metrics, 0);
      return metrics.lfMenuFont;
    }

    const font& font::system () {
      static font f = font((os::font)GetStockObject(SYSTEM_FONT));
      return f;
    }

    const font& font::system_bold () {
      static font f = font::system().with_thickness(font::bold);
      return f;
    }

    const font& font::system_small () {
      static font f = font::system().with_size(font::system().size() * 4 / 5);
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
      GetObject(id, sizeof (os::font_type), &info_);
    }

    font::font (os::font_type info_)
      : id(CreateFontIndirect(&info_))
      , info_(info_)
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
      GetObject(id, sizeof (os::font_type), &info_);
    }

    font::font (const font& rhs)
      : id(CreateFontIndirect(&rhs.info_))
      , info_(rhs.info_)
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
      return info_.lfFaceName;
    }

    font::size_type font::size () const {
      return info_.lfHeight;
    }

    font::Thickness font::thickness () const {
      return (Thickness)info_.lfWeight;
    }

    int font::rotation () const {
      return info_.lfOrientation;
    }

    bool font::italic () const {
      return info_.lfItalic != 0;
    }

    bool font::underline () const {
      return info_.lfUnderline != 0;
    }

    bool font::strikeout () const {
      return info_.lfStrikeOut != 0;
    }

    core::size::type font::line_height () const {
      return core::global::scale_from_native<core::size::type>(native_line_height());
    }

    font::size_type font::native_line_height () const {
      return info_.lfHeight;
    }

    font font::with_size (size_type sz) const {
      os::font_type newType = info_;
      newType.lfHeight = sz;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_thickness (Thickness t) const {
      os::font_type newType = info_;
      newType.lfWeight = t;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_rotation (int r) const {
      os::font_type newType = info_;
      newType.lfOrientation = r;
      newType.lfEscapement = r;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_italic (bool i) const {
      os::font_type newType = info_;
      newType.lfItalic = i;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_underline (bool u) const {
      os::font_type newType = info_;
      newType.lfUnderline = u;
      return font(CreateFontIndirect(&newType));
    }

    font font::with_strikeout (bool s) const {
      os::font_type newType = info_;
      newType.lfStrikeOut = s;
      return font(CreateFontIndirect(&newType));
    }

    bool font::operator== (const font& rhs) const {
      return ((info_.lfHeight == rhs.info_.lfHeight) &&
              (info_.lfEscapement == rhs.info_.lfEscapement) &&
              (info_.lfOrientation == rhs.info_.lfOrientation) &&
              (info_.lfWeight == rhs.info_.lfWeight) &&
              (info_.lfItalic == rhs.info_.lfItalic) &&
              (info_.lfUnderline == rhs.info_.lfUnderline) &&
              (info_.lfStrikeOut == rhs.info_.lfStrikeOut) &&
              (strcmp(info_.lfFaceName, rhs.info_.lfFaceName) == 0));
    }

    font& font::operator= (const font& rhs) {
      if (this != &rhs) {
        if (id) {
          DeleteObject(id);
        }
        info_ = rhs.info_;
        id = CreateFontIndirect(&info_);
      }
      return *this;
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
      return core::size(core::global::scale_from_native<core::size::type>(sz.cx), core::global::scale_from_native<core::size::type>(sz.cy));
    }
  }
}
#endif // GUIPP_WIN
