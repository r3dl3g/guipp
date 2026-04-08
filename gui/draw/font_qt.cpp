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

 #ifdef GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>

# include <QtGui/QFontDatabase>
# include <QtGui/QFontMetrics>

#include <logging/logger.h>
#include <util/string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/font.h"

namespace gui {

  namespace draw {

    const font& font::system () {
      static font f = font(QFontDatabase::systemFont(QFontDatabase::GeneralFont));
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
      static font f = font(QFontDatabase::systemFont(QFontDatabase::GeneralFont));
      return f;
    }

    const font& font::monospace () {
      static font f(QFontDatabase::systemFont(QFontDatabase::FixedFont));
      return f;
    }

    const font& font::serif () {
      static font f = [] () {
        os::font fn;
        fn.setStyleHint(QFont::StyleHint::Serif);
        return font(fn);
      }();
      return f;
    }

    const font& font::sans_serif () {
      static font f = [] () {
        os::font fn;
        fn.setStyleHint(QFont::StyleHint::SansSerif);
        return font(fn);
      }();
      return f;
    };

    font::font ()
      : info_(id)
    {}

    font::font (os::font id)
      : id(id)
      , info_(id)
    {}

    font::font (os::font_type i)
      : id(i.family(), i.pointSize(), i.weight(), i.italic())
      , info_(i)
    {}

    font::font (const std::string& name,
                font::size_type size,
                font::Thickness thickness,
                int rotation,
                bool italic,
                bool underline,
                bool strikeout)
      : id(QString::fromStdString(name), size, thickness, italic)
      , info_(id)
    {}

    font::font (const font& rhs)
      : id(rhs.id)
      , info_(rhs.info_)
    {}

    bool font::is_valid () const {
      return true;
    }

    font::~font ()
    {}

    font::operator os::font () const {
      return id;
    }

    std::string font::name () const {
      return info_.family().toStdString();
    }

    font::size_type font::size () const {
      return info_.pointSize();
    }

    font::Thickness font::thickness () const {
      return static_cast<Thickness>(info_.weight());
    }

    int font::rotation () const {
      return 0;
    }

    bool font::italic () const {
      return info_.italic();
    }

    bool font::underline () const {
      return info_.underline();
    }

    bool font::strikeout () const {
      return info_.strikeOut();
    }

    core::size::type font::line_height () const {
      return core::global::scale_from_native<core::size::type>(native_line_height());
    }

    font::size_type font::native_line_height () const {
      return QFontMetrics(id).lineSpacing();
    }

    font font::with_size (size_type sz) const {
      os::font f(id);
      f.setPointSize(sz);
      return font(f);
    }

    font font::with_thickness (Thickness t) const {
      os::font f(id);
      f.setWeight(static_cast<QFont::Weight>(t));
      return font(f);
    }

    font font::with_rotation (int r) const {
      return *this;
    }

    font font::with_italic (bool i) const {
      os::font f(id);
      f.setItalic(i);
      return font(f);
    }

    font font::with_underline (bool u) const {
      os::font f(id);
      f.setUnderline(u);
      return font(f);
    }

    font font::with_strikeout (bool s) const {
      os::font f(id);
      f.setStrikeOut(s);
      return font(f);
    }

    font& font::operator= (const font& rhs) {
      if (this == &rhs) {
        return *this;
      }
      id = rhs.id;
      info_ = rhs.info_;
      return *this;
    }

    bool font::operator== (const font& rhs) const {
      return ((info_.family() == rhs.info_.family()) &&
              (info_.pointSize() == rhs.info_.pointSize()) &&
              (info_.weight() == rhs.info_.weight()) &&
              (info_.italic() == rhs.info_.italic()) &&
              (info_.underline() == rhs.info_.underline()) &&
              (info_.strikeOut() == rhs.info_.strikeOut()));
    }

    std::ostream& operator<< (std::ostream& out, const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness() << ", " << f.italic();
      return out;
    }

    core::size font::get_text_size (const std::string& str) const {
      QFontMetrics metrics(id);
      auto sz = metrics.size(0, QString::fromStdString(str));
      return core::size(sz);
    }

  }
}
#endif // GUIPP_QT
