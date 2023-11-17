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
 * @brief     split window control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/split_view.h>


namespace gui {

  namespace ctrl {

    placeholder::placeholder ()
      : win(nullptr)
    {}

    void placeholder::layout (const core::rectangle& r) {
      if (win) {
        win->geometry(r, true, false);
      }
    }

    void placeholder::create (win::container& parent) {
      if (win) {
        win->create(parent);
      }
    }

    void placeholder::set (win::window* w) {
      win = w;
    }

  } // namespace ctrl

} // gui
