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

 #ifdef GUIPP_SDL

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>
#include <fontconfig/fontconfig.h>

#include <logging/logger.h>
#include <util/string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/font.h"
#include "gui/core/native.h"

#include <string>

std::string getSystemFontPath (const std::string& fontName) {
    FcConfig* config = FcInitLoadConfigAndFonts();
    FcPattern* pat = FcNameParse((const FcChar8*)fontName.c_str());
    FcConfigSubstitute(config, pat, FcMatchPattern);
    FcDefaultSubstitute(pat);

    std::string result = "";
    FcResult res;
    FcPattern* font = FcFontMatch(config, pat, &res);

    if (font) {
        FcChar8* file = NULL;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch) {
            result = (char*)file;
        }
        FcPatternDestroy(font);
    }
    FcPatternDestroy(pat);
    FcConfigDestroy(config);
    return result;
}

# define STD_FONT_SIZE 12

namespace gui {

  namespace draw {

    const font& font::system () {
      static font f("DejaVuSans.ttf", STD_FONT_SIZE);
      return f;
    }

    const font& font::menu () {
      static font f("DejaVuSans.ttf", STD_FONT_SIZE);
      return f;
    }

    const font& font::system_bold () {
      static font f("DejaVuSans-Bold.ttf", STD_FONT_SIZE, font::bold);
      return f;
    }

    const font& font::system_small () {
      static font f("DejaVuSansCondensed.ttf", STD_FONT_SIZE * 4 / 5);
      return f;
    }

    const font& font::monospace () {
      static font f("DejaVuSansMono.ttf", STD_FONT_SIZE);
      return f;
    }

    const font& font::serif () {
      static font f("DejaVuSerif.ttf", STD_FONT_SIZE);
      return f;
    }

    const font& font::sans_serif () {
      static font f("DejaVuSans.ttf", STD_FONT_SIZE);
      return f;
    }
  
    font::font ()
      : id{}
      , info_{}
    {}

    font::font (os::font_type i)
      : info_(i)
    {}

    font::font (const std::string& name,
                font::size_type size,
                font::Thickness thickness,
                int rotation,
                bool italic,
                bool underline,
                bool strikeout)
      : id{}
      , info_{name, size, static_cast<int>(thickness)}
    {}

    os::font font::os () const {
      init_font();
      return id;
    }

    void font::init_font () const {
      if (!is_valid() && core::native::sdl::get_font_renderer() && !info_.name.empty()) {
        id = os::font(FC_CreateFont(), font::delete_font);
        auto sz = static_cast<Uint32>(info_.size * core::global::get_scale_factor());
        auto path = getSystemFontPath(info_.name);
        logging::trace() << "FC_LoadFont(" << info_.name << ") with size " << sz << " from \"" << path << "\"";
        FC_LoadFont(id.get(), core::native::sdl::get_font_renderer(), path.c_str(), sz, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
      }
    }

    font::font (const font& rhs)
      : id{rhs.id}
      , info_(rhs.info_)
    {}

    bool font::is_valid () const {
      return static_cast<bool>(id);
    }

    font::~font () 
    {}

    void font::delete_font (FC_Font* info) {
      logging::trace() << "FC_FreeFont";
      FC_FreeFont(info);
      info = nullptr;
    }

    void font::destroy () 
    {}

    os::font_type font::font_type () const {
      return info_;
    }

    std::string font::name () const {
      return info_.name;
    }

    font::size_type font::size () const {
      return info_.size;
    }

    font::Thickness font::thickness () const {
      return static_cast<font::Thickness>(info_.thickness);
    }

    int font::rotation () const {
      return 0;
    }

    bool font::italic () const {
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
      id = rhs.id;
      info_ = rhs.info_;
      return *this;
    }

    bool font::operator== (const font& rhs) const {
      return (name() == rhs.name()) 
        && (size() == rhs.size()) 
        && (thickness() == rhs.thickness());
        // && (rotation() == rhs.rotation())
        // && (italic() == rhs.italic())
        // && (underline() == rhs.underline())
        // && (strikeout() == rhs.strikeout());
    }

    std::ostream& operator<< (std::ostream& out, const font& f) {
      out << f.name() << ", " << f.size() << ", " << f.thickness();
      return out;
    }

    core::size font::get_text_size (const std::string& str) const {
      auto h = line_height();
      return core::size(h /2 * str.size(), h);
    }

  }
}
#endif //GUIPP_SDL
