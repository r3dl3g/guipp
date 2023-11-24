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
 * @brief     image view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/ctrl/image.h>


using namespace gui::draw;
using namespace gui::win;
using namespace gui::core;

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    void image::create (container& parent,
                        const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
    }

    // --------------------------------------------------------------------------
    void image::paint (graphics& graph) {
      graph.clear(color::dark_gray);
      if (img.is_valid()) {
        graph.copy_from(img, core::point::zero);
      }
    }

    // --------------------------------------------------------------------------
    void image::set_image (const pixmap& source) {
      img = source;
      invalidate();
    }

    // --------------------------------------------------------------------------
    void image::set_image (pixmap&& source) {
      img = std::move(source);
      invalidate();
    }

    // --------------------------------------------------------------------------
    image::image () {
      on_paint(draw::paint(this, &image::paint));
    }

  } // ctrl

} // gui
