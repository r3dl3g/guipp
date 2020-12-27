/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>
#include <gui/win/container.h>
#include "gui/draw/bitmap.h"

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT image : public control {
    public:
      typedef control super;
      typedef no_erase_window_class<image> clazz;

      image ();

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void paint (const draw::graphics& graph);
      void set_image (const draw::pixmap& source);

    private:
      draw::pixmap img;
    };

  } // ctrl

} // gui
