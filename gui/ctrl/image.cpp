/**
 * @copyright (c) 2016-2019 Ing. Buero Rothfuss
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
 * @brief     C++ API: image view
 *
 * @file
 */

#include "image.h"

using namespace gui::draw;
using namespace gui::win;
using namespace gui::core;

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    void image::create (container& parent,
                        const core::rectangle& place) {
      super::create(/*clazz::get(), */parent, place);
    }

    // --------------------------------------------------------------------------
    void image::paint (const graphics& graph) {
      graph.clear(color::dark_gray);
      if (img.is_valid()) {
        graph.copy_from(img);
      }
    }

    // --------------------------------------------------------------------------
    void image::set_image (const pixmap& source) {
      img = source;
      invalidate();
    }

    // --------------------------------------------------------------------------
    image::image () {
      on_paint(draw::paint(this, &image::paint));
    }

  } // ctrl

} // gui
