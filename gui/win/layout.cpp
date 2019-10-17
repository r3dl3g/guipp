/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: layout
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/layout.h>
#include <gui/win/container.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    std::vector<win::window*> layout_base::get_children () {
      return main->get_children();
    }

    void layout_base::place_child (win::window* win, const core::rectangle& area) {
      win->place(area);
      win->set_visible();
    }

    bool layout_base::is_child_visible (win::window* win) {
      return win->is_visible();
    }

    void layout_base::hide_children (std::vector<win::window*>& children) {
      std::for_each(children.begin(), children.end(), [] (win::window*win) {
        win->set_visible(false);
      });
    }

    core::rectangle layout_base::get_layout_area () const {
      return main->client_area();
    }

    void layout_base::init (std::function<size_callback> f1) {
      main->on_show([&, f1]() {
        f1(main->client_area());
      });
      main->on_layout(std::move(f1));
    }

    void layout_base::init (std::function<size_callback> f1, std::function<show_callback> f2) {
      main->on_layout(std::move(f1));
      main->on_show(std::move(f2));
    }

  } // layout

} // gui
