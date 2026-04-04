/**
 * @copyright (c) 2016-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     Helper to use pen, brush and font
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#if GUIPP_JS
#include <util/ostreamfmt.h>
#include <iomanip>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/use.h"
#include "gui/draw/pen.h"
#include "gui/draw/brush.h"
#include "gui/draw/font.h"

namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen& p) {
      g.set("strokeStyle", ostreamfmt("#" << std::hex << std::setfill('0') << std::setw(6) << (p.color() & 0xffffff)));
      g.set("lineWidth", p.os_size());

      switch (p.cap()) {
        case pen::Cap::flat:   g.set("lineCap", "butt"); break;
        case pen::Cap::round:  g.set("lineCap", "round"); break;
        case pen::Cap::square: g.set("lineCap", "square"); break;
      }
      switch (p.join()) {
        case pen::Join::miter: g.set("lineJoin", "miter"); break;
        case pen::Join::round: g.set("lineJoin", "round"); break;
        case pen::Join::bevel: g.set("lineJoin", "bevel"); break;
      }

      // TBD: style
    }

    template<>
    void Use<brush>::set (const brush& b) {
      g.set("fillStyle", ostreamfmt("#" << std::hex << std::setfill('0') << std::setw(6) << (b.color() & 0xffffff)));
    }

    template<>
    void Use<font>::set (const font& f) {
      std::ostringstream str;
      if (f.italic()) {
        str << "itailc ";
      }
      if (f.thickness() != font::Thickness::regular) {
        str << (static_cast<int>(f.thickness()) * 100) << " ";
      }
      str << f.size() << "px " << f.name();
      g.set("font", str.str());
    }
    
  } // namespace draw
  
} // namespace gui

#endif // GUIPP_JS